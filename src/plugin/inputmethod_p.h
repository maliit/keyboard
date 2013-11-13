
#include "inputmethod.h"

#include "editor.h"
#include "keyboardgeometry.h"
#include "keyboardsettings.h"
#include "updatenotifier.h"

#include "logic/layoutupdater.h"
#include "logic/eventhandler.h"
#include "logic/wordengine.h"
#include "logic/languagefeatures.h"
#include "logic/dynamiclayout.h"

#include "ubuntuapplicationapiwrapper.h"

#include <maliit/plugins/abstractinputmethodhost.h>
#include <maliit/plugins/abstractpluginsetting.h>

#include <QtQuick>
#include <QStringList>
#include <qglobal.h>

#ifdef HAVE_QT_MOBILITY
#include "view/soundfeedback.h"
typedef MaliitKeyboard::SoundFeedback DefaultFeedback;
#else
#include "view/nullfeedback.h"
typedef MaliitKeyboard::NullFeedback DefaultFeedback;
#endif

using namespace MaliitKeyboard;

typedef QScopedPointer<Maliit::Plugins::AbstractPluginSetting> ScopedSetting;
typedef QSharedPointer<MKeyOverride> SharedOverride;
typedef QMap<QString, SharedOverride>::const_iterator OverridesIterator;

class Settings
{
public:
    ScopedSetting style;
};

class LayoutGroup
{
public:
    Logic::LayoutHelper helper;
    Logic::LayoutUpdater updater;
    Model::Layout model;
    Logic::EventHandler event_handler;

    explicit LayoutGroup();
};

LayoutGroup::LayoutGroup()
    : helper()
    , updater()
    , model()
    , event_handler(&model, &updater)
{}

QQuickView *createWindow(MAbstractInputMethodHost *host)
{
    QScopedPointer<QQuickView> view(new QQuickView);

    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    view->setFormat(format);
    view->setColor(QColor(Qt::transparent));

    host->registerWindow(view.data(), Maliit::PositionCenterBottom);

    return view.take();
}

class InputMethodPrivate
{
public:
    InputMethod* q;
    Editor editor;
    DefaultFeedback feedback;
    SharedStyle style;
    UpdateNotifier notifier;
    QMap<QString, SharedOverride> key_overrides;
    Settings settings;
    LayoutGroup layout;
    QRect windowGeometryRect;
    MAbstractInputMethodHost* host;
    QQuickView* view;
    UbuntuApplicationApiWrapper* applicationApiWrapper;

    bool autocapsEnabled;
    bool predictionEnabled;
    bool showWordRibbon;
    InputMethod::TextContentType contentType;
    QString systemLanguage;
    QString activeLanguage;
    QStringList enabledLanguages;
    Qt::ScreenOrientation appsCurrentOrientation;

    KeyboardGeometry *m_geometry;
    KeyboardSettings m_settings;

    explicit InputMethodPrivate(InputMethod * const _q,
                                MAbstractInputMethodHost *host)
        : q(_q)
        , editor(EditorOptions(), new Model::Text, new Logic::WordEngine, new Logic::LanguageFeatures)
        , feedback()
        , style(new Style)
        , notifier()
        , key_overrides()
        , settings()
        , layout()
        , host(host)
        , view(0)
        , applicationApiWrapper(new UbuntuApplicationApiWrapper)
        , autocapsEnabled(false)
        , predictionEnabled(false)
        , showWordRibbon(false)
        , contentType(InputMethod::FreeTextContentType)
        , systemLanguage("en")
        , activeLanguage(systemLanguage)
        , enabledLanguages(systemLanguage)
        , appsCurrentOrientation(qGuiApp->primaryScreen()->orientation())
        , m_geometry(new KeyboardGeometry(q))
        , m_settings()
    {
        applicationApiWrapper->setGeometryItem(m_geometry);

        view = createWindow(host);

        editor.setHost(host);

        layout.updater.setLayout(&layout.helper);

        layout.updater.setStyle(style);
        feedback.setStyle(style);

        const QSize &screen_size(view->screen()->size());
        layout.helper.setScreenSize(screen_size);
        layout.helper.setAlignment(Logic::LayoutHelper::Bottom);

        QObject::connect(&layout.event_handler, SIGNAL(wordCandidatePressed(WordCandidate)),
                         &layout.updater, SLOT( onWordCandidatePressed(WordCandidate) ));

        QObject::connect(&layout.event_handler, SIGNAL(wordCandidateReleased(WordCandidate)),
                         &layout.updater, SLOT( onWordCandidateReleased(WordCandidate) ));

        QObject::connect(&editor,  SIGNAL(preeditEnabledChanged(bool)),
                         &layout.updater, SLOT(setWordRibbonVisible(bool)));

        QObject::connect(&layout.updater, SIGNAL(wordCandidateSelected(QString)),
                         editor.wordEngine(),  SLOT(onWordCandidateSelected(QString)));

        QObject::connect(&layout.updater, SIGNAL(languageChanged(QString)),
                         editor.wordEngine(),  SLOT(onLanguageChanged(QString)));

        QObject::connect(&layout.updater, SIGNAL(languageChanged(QString)),
                         &editor,  SLOT(onLanguageChanged(const QString&)));

        QObject::connect(&layout.helper, SIGNAL(stateChanged(Model::Layout::State)),
                         &layout.model,  SLOT(setState(Model::Layout::State)));

        connectToNotifier();

    #ifdef DISABLED_FLAGS_FROM_SURFACE
        view->setFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint
                          | Qt::X11BypassWindowManagerHint | Qt::WindowDoesNotAcceptFocus);
    #endif
        view->setWindowState(Qt::WindowNoState);

        QSurfaceFormat format;
        format.setAlphaBufferSize(8);
        view->setFormat(format);
        view->setColor(QColor(Qt::transparent));

        view->setVisible(false);

        // TODO: Figure out whether two views can share one engine.
        QQmlEngine *const engine(view->engine());
        engine->addImportPath(UBUNTU_KEYBOARD_DATA_DIR);
        setContextProperties(engine->rootContext());

        // following used to help shell identify the OSK surface
        view->setProperty("role", applicationApiWrapper->oskWindowRole());
        view->setTitle("MaliitOnScreenKeyboard");

        // workaround: resizeMode not working in current qpa imlementation
        // http://qt-project.org/doc/qt-5.0/qtquick/qquickview.html#ResizeMode-enum
        view->setResizeMode(QQuickView::SizeRootObjectToView);
    }

    ~InputMethodPrivate()
    {
        delete applicationApiWrapper;
    }

    Logic::LayoutHelper::Orientation screenToMaliitOrientation(Qt::ScreenOrientation screenOrientation) const
    {
        switch (screenOrientation) {
        case Qt::LandscapeOrientation:
        case Qt::InvertedLandscapeOrientation:
            return Logic::LayoutHelper::Landscape;
            break;
        case Qt::PortraitOrientation:
        case Qt::InvertedPortraitOrientation:
        case Qt::PrimaryOrientation:
        default:
            return Logic::LayoutHelper::Portrait;
        }

        return Logic::LayoutHelper::Portrait;
    }

    void setLayoutOrientation(Qt::ScreenOrientation screenOrientation)
    {
        Logic::LayoutHelper::Orientation orientation = uiConst->screenToMaliitOrientation(screenOrientation);

        layout.updater.setOrientation(orientation);

        windowGeometryRect = m_geometry->visibleRect().toRect();

        m_geometry->setOrientation(screenOrientation);
    }

    void updateWordRibbon()
    {
        bool enabled = predictionEnabled;
        enabled = enabled && (contentType == InputMethod::FreeTextContentType);
        enabled = enabled && m_settings.predictiveText();

        layout.helper.wordRibbon()->setEnabled( enabled );
        Q_EMIT q->wordRibbonEnabledChanged( enabled );

        if (enabled != showWordRibbon) {
            showWordRibbon = enabled;
            Q_EMIT q->showWordRibbonChanged(showWordRibbon);
        }

        setLayoutOrientation(appsCurrentOrientation);
    }

    void connectToNotifier()
    {
    #ifdef TEMP_DISABLED
        QObject::connect(&notifier, SIGNAL(cursorPositionChanged(int, QString)),
                         &editor,   SLOT(onCursorPositionChanged(int, QString)));
    #endif
        QObject::connect(&notifier,      SIGNAL(keysOverriden(Logic::KeyOverrides, bool)),
                         &layout.helper, SLOT(onKeysOverriden(Logic::KeyOverrides, bool)));
    }

    void setContextProperties(QQmlContext *qml_context)
    {
        qml_context->setContextProperty("maliit_input_method", q);
        qml_context->setContextProperty("maliit_geometry", m_geometry);
        qml_context->setContextProperty("maliit_layout", &layout.model);
        qml_context->setContextProperty("maliit_event_handler", &layout.event_handler);
        qml_context->setContextProperty("maliit_wordribbon", layout.helper.wordRibbon());
    }


    /*
     * register settings
     */

    void registerStyleSetting(MAbstractInputMethodHost *host)
    {
        QVariantMap attributes;
        QStringList available_styles = style->availableProfiles();
        attributes[Maliit::SettingEntryAttributes::defaultValue] = MALIIT_DEFAULT_PROFILE;
        attributes[Maliit::SettingEntryAttributes::valueDomain] = available_styles;
        attributes[Maliit::SettingEntryAttributes::valueDomainDescriptions] = available_styles;

        settings.style.reset(host->registerPluginSetting("current_style",
                                                            QT_TR_NOOP("Keyboard style"),
                                                            Maliit::StringType,
                                                            attributes));

        QObject::connect(settings.style.data(), SIGNAL(valueChanged()), q, SLOT(onStyleSettingChanged()));

        // Call manually for the first time to initialize dependent values:
        q->onStyleSettingChanged();
    }

    void registerFeedbackSetting()
    {
        QObject::connect(&m_settings, SIGNAL(keyPressFeedbackChanged()),
                         q, SLOT(onFeedbackSettingChanged()));
        feedback.setEnabled(m_settings.keyPressFeedback());
    }

    void registerAutoCorrectSetting()
    {
        QObject::connect(&m_settings, SIGNAL(autoCompletionChanged()),
                         q, SLOT(onAutoCorrectSettingChanged()));
        editor.setAutoCorrectEnabled(m_settings.autoCompletion());
    }

    void registerAutoCapsSetting()
    {
        QObject::connect(&m_settings, SIGNAL(autoCapitalizationChanged()),
                         q, SLOT(updateAutoCaps()));
    }

    void registerWordEngineSetting()
    {
        QObject::connect(&m_settings, SIGNAL(predictiveTextChanged()),
                         q, SLOT(updateWordEngine()));
    #ifndef DISABLE_PREEDIT
        editor.wordEngine()->setEnabled(m_settings.predictiveText());
    #else
        editor.wordEngine()->setEnabled(false);
    #endif
    }

    void registerEnabledLanguages()
    {
        QObject::connect(&m_settings, SIGNAL(enabledLanguagesChanged()),
                         q, SLOT(onEnabledLanguageSettingsChanged()));
        q->onEnabledLanguageSettingsChanged();

        registerSystemLanguage();
        q->setActiveLanguage(systemLanguage);
    }

    //! \brief registerSystemLanguage reads the language from the system
    //! FIXME check if the language is supported - if not use "en" as fallback
    void registerSystemLanguage()
    {
        systemLanguage = QString(getenv("LANGUAGE"));
        systemLanguage.truncate(2);
        Q_EMIT q->systemLanguageChanged(systemLanguage);
    }

    void onScreenSizeChange(const QSize &size)
    {
        layout.helper.setScreenSize(size);
    }

    void closeOskWindow()
    {
        if (!view->isVisible())
            return;

        host->notifyImInitiatedHiding();

        m_geometry->setShown(false);

        layout.updater.resetOnKeyboardClosed();
        editor.clearPreedit();

        view->setVisible(false);

        applicationApiWrapper->reportOSKInvisible();
    }

    void truncateEnabledLanguageLocales(QStringList locales)
    {
        enabledLanguages.clear();
        foreach (QString locale, locales) {
            locale.truncate(2);
            enabledLanguages << locale;
        }
    }
};
