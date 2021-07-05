/*
 * Copyright (C) 2013-2016 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "inputmethod.h"
#include "coreutils.h"

#include "logic/layoutupdater.h"
#include "editor.h"
#include "greeterstatus.h"
#include "keyboardgeometry.h"
#include "keyboardsettings.h"

#include "logic/eventhandler.h"
#include "logic/wordengine.h"

#include <maliit/plugins/abstractinputmethodhost.h>
#include <maliit/plugins/abstractpluginsetting.h>

#include <QtQuick>
#include <QStringList>
#include <qglobal.h>
#include <QDebug>


namespace
{
// Qt::WindowType enum has no option for an Input Method window type. This is a magic value
// used by ubuntumirclient QPA for special clients to request input method windows from Mir.
const Qt::WindowType InputMethodWindowType = (Qt::WindowType)(0x00000080 | Qt::WindowType::Window);
}

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

    bool autocapsEnabled;
    bool wordEngineEnabled;
    InputMethod::TextContentType contentType;
    QString activeLanguage;
    QString previousLanguage;
    QStringList enabledLanguages;
    Qt::ScreenOrientation appsCurrentOrientation;
    QString keyboardState;
    bool hasSelection;

    QString preedit;

    KeyboardGeometry *m_geometry;
    KeyboardSettings m_settings;
    GreeterStatus *m_greeterStatus;

    WordRibbon* wordRibbon;

    int previous_position;

    QStringList pluginPaths;
    QString currentPluginPath;

    explicit InputMethodPrivate(InputMethod * const _q,
                                MAbstractInputMethodHost *host)
        : q(_q)
        , editor(EditorOptions(), new Model::Text, new Logic::WordEngine)
        , event_handler()
        , host(host)
        , view(0)
        , autocapsEnabled(false)
        , wordEngineEnabled(false)
        , contentType(InputMethod::FreeTextContentType)
        , activeLanguage("en")
        , previousLanguage("")
        , enabledLanguages(activeLanguage)
        , appsCurrentOrientation(qGuiApp->primaryScreen()->orientation())
        , keyboardState("CHARACTERS")
        , hasSelection(false)
        , preedit("")
        , m_geometry(new KeyboardGeometry(q))
        , m_settings()
        , m_greeterStatus(new GreeterStatus())
        , wordRibbon(new WordRibbon)
        , previous_position(-1)
    {
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

        QSurfaceFormat format = view->format();
        format.setAlphaBufferSize(8);
        view->setFormat(format);
        view->setColor(QColor(Qt::transparent));

        updatePluginPaths();

        // TODO: Figure out whether two views can share one engine.
        QQmlEngine *const engine(view->engine());

        QString prefix = qgetenv("KEYBOARD_PREFIX_PATH");
        if (!prefix.isEmpty()) {
            engine->addImportPath(prefix + QDir::separator() + LOMIRI_KEYBOARD_DATA_DIR);
            engine->addImportPath(prefix + QDir::separator() + QString(LOMIRI_KEYBOARD_DATA_DIR) + QDir::separator() + "keys");
        } else {
            engine->addImportPath(LOMIRI_KEYBOARD_DATA_DIR);
            engine->addImportPath(QString(LOMIRI_KEYBOARD_DATA_DIR) + QDir::separator() + "keys");
        }

        setContextProperties(engine->rootContext());

        // workaround: resizeMode not working in current qpa imlementation
        // http://qt-project.org/doc/qt-5.0/qtquick/qquickview.html#ResizeMode-enum
        view->setResizeMode(QQuickView::SizeRootObjectToView);

        if (QGuiApplication::platformName() == "ubuntumirclient") {
            view->setFlags(InputMethodWindowType); /* Mir-only OSK window type */
        }
        // When keyboard geometry changes, update the window's input mask
        QObject::connect(m_geometry, &KeyboardGeometry::visibleRectChanged, view, [this]() {
            view->setMask(m_geometry->visibleRect().toRect());
        });
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

    void updatePluginPaths()
    {
        pluginPaths.clear();
        QString prefix = qgetenv("KEYBOARD_PREFIX_PATH");
        if (!prefix.isEmpty()) {
            pluginPaths.append(prefix + QDir::separator() + QString(LOMIRI_KEYBOARD_DATA_DIR) + QDir::separator() + "lib");
        } else {
            pluginPaths.append(QString(LOMIRI_KEYBOARD_DATA_DIR) + QDir::separator() + "lib");
        }
        pluginPaths.append(m_settings.pluginPaths());
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
    
    void registerEnableMagnifier()
    {
        QObject::connect(&m_settings, SIGNAL(enableMagnifierChanged(bool)),
                         q, SIGNAL(enableMagnifierChanged()));
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

    void registerPreviousLanguage()
    {
        QObject::connect(&m_settings, SIGNAL(previousLanguageChanged(QString)),
                         q, SLOT(setPreviousLanguage(QString)));

        previousLanguage = m_settings.previousLanguage();
        q->setPreviousLanguage(previousLanguage);
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

    void registerStayHidden()
    {
        QObject::connect(&m_settings, SIGNAL(stayHiddenChanged(bool)),
                         q, SLOT(hide()));
    }

    void registerPluginPaths()
    {
        QObject::connect(&m_settings, SIGNAL(pluginPathsChanged(QStringList)),
                        q, SLOT(onPluginPathsChanged(QStringList)));
    }

    void registerOpacity()
    {
        QObject::connect(&m_settings, SIGNAL(opacityChanged(double)),
                        q, SIGNAL(opacityChanged(double)));
    }
    
    void registerTheme()
    {
        QObject::connect(&m_settings, SIGNAL(themeChanged(QString)),
                        q, SIGNAL(themeChanged(QString)));
    }

    void closeOskWindow()
    {
        if (!view->isVisible())
            return;

        host->notifyImInitiatedHiding();

        m_geometry->setShown(false);

        editor.clearPreedit();

        view->setVisible(false);
    }
};
