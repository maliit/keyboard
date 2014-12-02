
#include "inputmethod.h"

#include "logic/layoutupdater.h"
#include "editor.h"
#include "greeterstatus.h"
#include "keyboardgeometry.h"
#include "keyboardsettings.h"

#include "logic/eventhandler.h"
#include "logic/wordengine.h"

#include "ubuntuapplicationapiwrapper.h"

#include <maliit/plugins/abstractinputmethodhost.h>
#include <maliit/plugins/abstractpluginsetting.h>

#include <QtQuick>
#include <QStringList>
#include <qglobal.h>
#include <QDebug>

using namespace MaliitKeyboard;

typedef QScopedPointer<Maliit::Plugins::AbstractPluginSetting> ScopedSetting;
typedef QSharedPointer<MKeyOverride> SharedOverride;
typedef QMap<QString, SharedOverride>::const_iterator OverridesIterator;

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
    SharedOverride actionKeyOverrider;
    Logic::EventHandler event_handler;
    MAbstractInputMethodHost* host;
    QQuickView* view;
    UbuntuApplicationApiWrapper* applicationApiWrapper;

    bool autocapsEnabled;
    bool wordEngineEnabled;
    InputMethod::TextContentType contentType;
    QString activeLanguage;
    QStringList enabledLanguages;
    Qt::ScreenOrientation appsCurrentOrientation;
    QString keyboardState;
    bool hasSelection;

    KeyboardGeometry *m_geometry;
    KeyboardSettings m_settings;
    GreeterStatus *m_greeterStatus;

    WordRibbon* wordRibbon;

    int previous_position;

    explicit InputMethodPrivate(InputMethod * const _q,
                                MAbstractInputMethodHost *host)
        : q(_q)
        , editor(EditorOptions(), new Model::Text, new Logic::WordEngine)
        , event_handler()
        , host(host)
        , view(0)
        , applicationApiWrapper(new UbuntuApplicationApiWrapper)
        , autocapsEnabled(false)
        , wordEngineEnabled(false)
        , contentType(InputMethod::FreeTextContentType)
        , activeLanguage("en")
        , enabledLanguages(activeLanguage)
        , appsCurrentOrientation(qGuiApp->primaryScreen()->orientation())
        , keyboardState("CHARACTERS")
        , hasSelection(false)
        , m_geometry(new KeyboardGeometry(q))
        , m_settings()
        , m_greeterStatus(new GreeterStatus())
        , wordRibbon(new WordRibbon)
        , previous_position(-1)
    {
        applicationApiWrapper->setGeometryItem(m_geometry);

        view = createWindow(host);

        editor.setHost(host);

        //! connect wordRibbon
        QObject::connect(&event_handler, SIGNAL(wordCandidatePressed(WordCandidate)),
                         wordRibbon, SLOT( onWordCandidatePressed(WordCandidate) ));

        QObject::connect(&event_handler, SIGNAL(wordCandidateReleased(WordCandidate)),
                         wordRibbon, SLOT( onWordCandidateReleased(WordCandidate) ));

        QObject::connect(&editor,  SIGNAL(wordCandidatesChanged(WordCandidateList)),
                         wordRibbon, SLOT(onWordCandidatesChanged(WordCandidateList)));

        QObject::connect(wordRibbon, SIGNAL(wordCandidateSelected(QString)),
                         &editor,  SLOT(replaceAndCommitPreedit(QString)));

        QObject::connect(wordRibbon, SIGNAL(userCandidateSelected(QString)),
                         &editor,  SLOT(addToUserDictionary(QString)));

        QObject::connect(&editor,  SIGNAL(preeditEnabledChanged(bool)),
                         wordRibbon, SLOT(setWordRibbonVisible(bool)));

        QObject::connect(wordRibbon, SIGNAL(wordCandidateSelected(QString)),
                         editor.wordEngine(),  SLOT(onWordCandidateSelected(QString)));

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
        m_geometry->setOrientation(screenOrientation);
    }

    void setContextProperties(QQmlContext *qml_context)
    {
        qml_context->setContextProperty("maliit_input_method", q);
        qml_context->setContextProperty("maliit_geometry", m_geometry);
        qml_context->setContextProperty("maliit_event_handler", &event_handler);
        qml_context->setContextProperty("maliit_wordribbon", wordRibbon);
        qml_context->setContextProperty("maliit_word_engine", editor.wordEngine());
        qml_context->setContextProperty("greeter_status", m_greeterStatus);
    }


    /*
     * register settings
     */
    void registerAudioFeedbackSoundSetting()
    {
        QObject::connect(&m_settings, SIGNAL(keyPressAudioFeedbackSoundChanged(QString)),
                         q, SIGNAL(audioFeedbackSoundChanged(QString)));
    }

    void registerAudioFeedbackSetting()
    {
        QObject::connect(&m_settings, SIGNAL(keyPressAudioFeedbackChanged(bool)),
                         q, SIGNAL(useAudioFeedbackChanged()));
    }

    void registerHapticFeedbackSetting()
    {
        QObject::connect(&m_settings, SIGNAL(keyPressHapticFeedbackChanged(bool)),
                         q, SIGNAL(useHapticFeedbackChanged()));
    }

    void registerAutoCorrectSetting()
    {
        QObject::connect(&m_settings, SIGNAL(autoCompletionChanged(bool)),
                         q, SLOT(onAutoCorrectSettingChanged()));
        editor.setAutoCorrectEnabled(m_settings.autoCompletion());
    }

    void registerAutoCapsSetting()
    {
        QObject::connect(&m_settings, SIGNAL(autoCapitalizationChanged(bool)),
                         q, SLOT(updateAutoCaps()));
    }

    void registerWordEngineSetting()
    {
        QObject::connect(&m_settings, SIGNAL(predictiveTextChanged(bool)),
                         editor.wordEngine(), SLOT(setWordPredictionEnabled(bool)));
        editor.wordEngine()->setWordPredictionEnabled(m_settings.predictiveText());

        QObject::connect(&m_settings, SIGNAL(spellCheckingChanged(bool)),
                         editor.wordEngine(), SLOT(setSpellcheckerEnabled(bool)));
        editor.wordEngine()->setSpellcheckerEnabled(m_settings.spellchecking());
    }

    void registerActiveLanguage()
    {
        QObject::connect(&m_settings, SIGNAL(activeLanguageChanged(QString)),
                         q, SLOT(setActiveLanguage(QString)));

        activeLanguage = m_settings.activeLanguage();
        qDebug() << "inputmethod_p.h registerActiveLanguage(): activeLanguage is:" << activeLanguage;
        q->setActiveLanguage(activeLanguage);
    }

    void registerEnabledLanguages()
    {
        QObject::connect(&m_settings, SIGNAL(enabledLanguagesChanged(QStringList)),
                         q, SLOT(onEnabledLanguageSettingsChanged()));
        q->onEnabledLanguageSettingsChanged();

        //registerSystemLanguage();
        //q->setActiveLanguage(activeLanguage);
    }

    void registerDoubleSpaceFullStop()
    {
        QObject::connect(&m_settings, SIGNAL(doubleSpaceFullStopChanged(bool)),
                         q, SLOT(onDoubleSpaceSettingChanged()));
        editor.setDoubleSpaceFullStopEnabled(m_settings.doubleSpaceFullStop());
    }

    void closeOskWindow()
    {
        if (!view->isVisible())
            return;

        host->notifyImInitiatedHiding();

        m_geometry->setShown(false);

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
