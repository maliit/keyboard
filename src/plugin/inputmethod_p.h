
#include "inputmethod.h"

#include "editor.h"

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
    ScopedSetting feedback;
    ScopedSetting auto_correct;
    ScopedSetting auto_caps;
    ScopedSetting word_engine;
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

    bool predictionEnabled;
    Maliit::TextContentType contentType;
    QString activeLanguageId;

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
        , predictionEnabled(false)
        , contentType(Maliit::FreeTextContentType)
        , activeLanguageId("en_us")
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

    void setLayoutOrientation(Qt::ScreenOrientation screenOrientation)
    {
        Logic::LayoutHelper::Orientation orientation = uiConst->screenToMaliitOrientation(screenOrientation);

        layout.updater.setOrientation(orientation);

        windowGeometryRect = uiConst->windowGeometryRect( screenOrientation );

        keyboardVisibleRect = windowGeometryRect.adjusted(0,uiConst->invisibleTouchAreaHeight(orientation),0,0);

        // qpa does not rotate the coordinate system
        windowGeometryRect = qGuiApp->primaryScreen()->mapBetween(
                        screenOrientation,
                        qGuiApp->primaryScreen()->primaryOrientation(),
                        windowGeometryRect);


        view->setGeometry(windowGeometryRect);

        if (qmlRootItem->property("shown").toBool()) {
            host->setScreenRegion(QRegion(keyboardVisibleRect));

            QRect rect(keyboardVisibleRect);
            rect.moveTop( windowGeometryRect.height() - keyboardVisibleRect.height() );
            host->setInputMethodArea(rect, view);
        }

        qmlRootItem->setProperty("contentOrientation", screenOrientation);

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

    void updateKeyboardOrientation()
    {
        setLayoutOrientation(QGuiApplication::primaryScreen()->orientation());
    }

    void updateWordRibbon()
    {
        layout.helper.wordRibbon()->setEnabled( predictionEnabled );
        Q_EMIT q->wordRibbonEnabledChanged( predictionEnabled );
        qmlRootItem->setProperty("wordribbon_visible", predictionEnabled );

        updateKeyboardOrientation();
    }

    /*
     * changes keyboard layout
     * called directly to show URL or num layout for special contentTypes,
     * does not change the current language id / activeView
     */

    void setActiveKeyboardId(const QString& id)
    {
        // FIXME: Perhaps better to let both LayoutUpdater share the same KeyboardLoader instance?
        layout.updater.setActiveKeyboardId(id);
        layout.model.setActiveView(id);
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

    void registerFeedbackSetting(MAbstractInputMethodHost *host)
    {
        QVariantMap attributes;
        attributes[Maliit::SettingEntryAttributes::defaultValue] = true;

        settings.feedback.reset(host->registerPluginSetting("feedback_enabled",
                                                               QT_TR_NOOP("Feedback enabled"),
                                                               Maliit::BoolType,
                                                               attributes));

        QObject::connect(settings.feedback.data(), SIGNAL(valueChanged()), q, SLOT(onFeedbackSettingChanged()));

        feedback.setEnabled(settings.feedback->value().toBool());
    }

    void registerAutoCorrectSetting(MAbstractInputMethodHost *host)
    {
        QVariantMap attributes;
        attributes[Maliit::SettingEntryAttributes::defaultValue] = true;

        settings.auto_correct.reset(host->registerPluginSetting("auto_correct_enabled",
                                                                   QT_TR_NOOP("Auto-correct enabled"),
                                                                   Maliit::BoolType,
                                                                   attributes));

        QObject::connect(settings.auto_correct.data(), SIGNAL(valueChanged()), q, SLOT(onAutoCorrectSettingChanged()));

        editor.setAutoCorrectEnabled(settings.auto_correct->value().toBool());
    }

    void registerAutoCapsSetting(MAbstractInputMethodHost *host)
    {
        QVariantMap attributes;
        attributes[Maliit::SettingEntryAttributes::defaultValue] = true;

        settings.auto_caps.reset(host->registerPluginSetting("auto_caps_enabled",
                                                                QT_TR_NOOP("Auto-capitalization enabled"),
                                                                Maliit::BoolType,
                                                                attributes));

        QObject::connect(settings.auto_caps.data(), SIGNAL(valueChanged()), q, SLOT(onAutoCapsSettingChanged()));

        editor.setAutoCapsEnabled(settings.auto_caps->value().toBool());
    }

    void registerWordEngineSetting(MAbstractInputMethodHost *host)
    {
        QVariantMap attributes;
        attributes[Maliit::SettingEntryAttributes::defaultValue] = false;

        settings.word_engine.reset(host->registerPluginSetting("word_engine_enabled",
                                                                  QT_TR_NOOP("Error correction/word prediction enabled"),
                                                                  Maliit::BoolType,
                                                                  attributes));

        QObject::connect(settings.word_engine.data(), SIGNAL(valueChanged()), q, SLOT(updateWordEngine()));

        Q_EMIT q->wordEngineEnabledChanged( settings.word_engine.data()->value().toBool() );

    #ifndef DISABLE_PREEDIT
        editor.wordEngine()->setEnabled(settings.word_engine->value().toBool());
    #else
        editor.wordEngine()->setEnabled(false);
    #endif
    }

    void onScreenSizeChange(const QSize &size)
    {
        layout.helper.setScreenSize(size);

    #ifdef TEMP_DISABLED
        updateKeyboardOrientation();
    #endif
    }

};

} // namespace

