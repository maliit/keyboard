
#include "inputmethod.h"

#include "editor.h"
#include "keyboadsettings.h"
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

namespace MaliitKeyboard {

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
    QQuickItem* qmlRootItem;
    Editor editor;
    DefaultFeedback feedback;
    SharedStyle style;
    UpdateNotifier notifier;
    QMap<QString, SharedOverride> key_overrides;
    Settings settings;
    LayoutGroup layout;
    QRect windowGeometryRect;
    QRect keyboardVisibleRect;
    MAbstractInputMethodHost* host;
    QQuickView* view;
    UbuntuApplicationApiWrapper* applicationApiWrapper;

    bool autocapsEnabled;
    bool predictionEnabled;
    Maliit::TextContentType contentType;
    QString activeLanguage;
    QStringList enabledLanguages;
    Qt::ScreenOrientation appsCurrentOrientation;

    KeyboadSettings m_settings;

    explicit InputMethodPrivate(InputMethod * const _q,
                                MAbstractInputMethodHost *host)
        : q(_q)
        , qmlRootItem(0)
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
        , contentType(Maliit::FreeTextContentType)
        , appsCurrentOrientation(qGuiApp->primaryScreen()->orientation())
        , m_settings()
    {
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

        QObject::connect(view, SIGNAL(statusChanged(QQuickView::Status)),
                        q, SLOT(onQQuickViewStatusChanged(QQuickView::Status)));

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
        Logic::LayoutHelper::Orientation orientation = screenToMaliitOrientation(screenOrientation);

        layout.updater.setOrientation(orientation);

        windowGeometryRect = uiConst->windowGeometryRect( screenOrientation );

        keyboardVisibleRect = windowGeometryRect.adjusted(0,uiConst->invisibleTouchAreaHeight(orientation),0,0);

        qmlRootItem->setProperty("height", windowGeometryRect.height());
        qmlRootItem->setProperty("keypadHeight", keyboardVisibleRect.height());
        qmlRootItem->setProperty("contentOrientation", screenOrientation);

        // qpa does not rotate the coordinate system
        windowGeometryRect = qGuiApp->primaryScreen()->mapBetween(
                        screenOrientation,
                        qGuiApp->primaryScreen()->primaryOrientation(),
                        windowGeometryRect);

        if (qmlRootItem->property("shown").toBool()) {
            host->setScreenRegion(QRegion(keyboardVisibleRect));

            QRect rect(keyboardVisibleRect);
            rect.moveTop( windowGeometryRect.height() - keyboardVisibleRect.height() );
            host->setInputMethodArea(rect, view);
        }

        if (qmlRootItem->property("shown").toBool()) {
            applicationApiWrapper->reportOSKInvisible();

            qDebug() << "keyboard is reporting: total <x y w h>: <"
                     << windowGeometryRect.x()
                     << windowGeometryRect.y()
                     << windowGeometryRect.width()
                     << windowGeometryRect.height()
                     << "> and visible <"
                     << keyboardVisibleRect.x()
                     << keyboardVisibleRect.y()
                     << keyboardVisibleRect.width()
                     << keyboardVisibleRect.height()
                     << "> to the app manager.";

            // report the visible part as input trap, the invisible part can click through, e.g. browser url bar
            applicationApiWrapper->reportOSKVisible(
                        keyboardVisibleRect.x(),
                        keyboardVisibleRect.y(),
                        keyboardVisibleRect.width(),
                        keyboardVisibleRect.height()
                        );
        }
    }

    void updateWordRibbon()
    {
        layout.helper.wordRibbon()->setEnabled( predictionEnabled );
        Q_EMIT q->wordRibbonEnabledChanged( predictionEnabled );
        qmlRootItem->setProperty("wordribbon_visible", predictionEnabled );

        setLayoutOrientation(appsCurrentOrientation);
    }

    /*
     * changes keyboard layout
     * called directly to show URL or num layout for special contentTypes,
     * does not change the current language id / activeView
     */

    void setActiveKeyboardId(const QString& id)
    {
        qmlRootItem->setProperty("layoutId", id);
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
        truncateEnabledLanguageLocales(m_settings.enabledLanguages());
        Q_EMIT q->enabledLanguagesChanged(enabledLanguages);

        registerActiveLanguage();
    }

    void registerActiveLanguage()
    {
        activeLanguage = QString(getenv("LANGUAGE"));
        activeLanguage.truncate(2);

        Q_EMIT q->activeLanguageChanged(activeLanguage);
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

        qmlRootItem->setProperty("shown", false);

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

} // namespace

