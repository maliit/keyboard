/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: Mohammad Anwari <Mohammad.Anwari@nokia.com>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Nokia Corporation nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "inputmethod.h"
#include "editor.h"
#include "updatenotifier.h"
#include "maliitcontext.h"
#include "ubuntuapplicationapiwrapper.h"

#include "models/key.h"
#include "models/keyarea.h"
#include "models/wordribbon.h"
#include "models/layout.h"

#include "logic/layouthelper.h"
#include "logic/layoutupdater.h"
#include "logic/wordengine.h"
#include "logic/style.h"
#include "logic/languagefeatures.h"
#include "logic/eventhandler.h"
#include "logic/keyareaconverter.h"
#include "logic/dynamiclayout.h"

#include "view/setup.h"

#ifdef HAVE_QT_MOBILITY
#include "view/soundfeedback.h"
typedef MaliitKeyboard::SoundFeedback DefaultFeedback;
#else
#include "view/nullfeedback.h"
typedef MaliitKeyboard::NullFeedback DefaultFeedback;
#endif

#include <maliit/plugins/subviewdescription.h>
#include <maliit/plugins/abstractpluginsetting.h>
#include <maliit/plugins/updateevent.h>
#include <maliit/plugins/abstractinputmethodhost.h>
#include <maliit/namespace.h>

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>
#include <QtQuick>

class MImUpdateEvent;

namespace MaliitKeyboard {

typedef QScopedPointer<Maliit::Plugins::AbstractPluginSetting> ScopedSetting;
typedef QSharedPointer<MKeyOverride> SharedOverride;
typedef QMap<QString, SharedOverride>::const_iterator OverridesIterator;

namespace {

const QString g_maliit_keyboard_qml(UBUNTU_KEYBOARD_DATA_DIR "/maliit-keyboard.qml");
const QString g_maliit_keyboard_extended_qml(UBUNTU_KEYBOARD_DATA_DIR "/maliit-keyboard-extended.qml");
const QString g_maliit_magnifier_qml(UBUNTU_KEYBOARD_DATA_DIR "/maliit-magnifier.qml");

Key overrideToKey(const SharedOverride &override)
{
    Key key;

    key.rLabel().setText(override->label());
    key.setIcon(override->icon().toUtf8());
    // TODO: hightlighted and enabled information are not available in
    // Key. Should we just really create a KeyOverride model?

    return key;
}

} // unnamed namespace

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
#ifdef EXTENDED_SURFACE_TEMP_DISABLED
    SharedSurface extended_surface;
    SharedSurface magnifier_surface;
#endif
    Editor editor;
    DefaultFeedback feedback;
    SharedStyle style;
    UpdateNotifier notifier;
    QMap<QString, SharedOverride> key_overrides;
    Settings settings;
    LayoutGroup layout;
    LayoutGroup extended_layout;
    Model::Layout magnifier_layout;
    MaliitContext context;
    QRect windowGeometryRect;
    QRect keyboardVisibleRect;
    MAbstractInputMethodHost* host;
    QQuickView* view;
    UbuntuApplicationApiWrapper* applicationApiWrapper;

    bool predictionEnabled;
    Maliit::TextContentType contentType;
    QString activeLanguageId;

    explicit InputMethodPrivate(InputMethod * const q,
                                MAbstractInputMethodHost *host);
    void setLayoutOrientation(Qt::ScreenOrientation qtOrientation);
    void updateKeyboardOrientation();
    void updateWordRibbon();

    void setActiveKeyboardId(const QString& id);
    void connectToNotifier();
    void setContextProperties(QQmlContext *qml_context);
};


InputMethodPrivate::InputMethodPrivate(InputMethod *const _q,
                                       MAbstractInputMethodHost *host)
  //    : surface_factory(host->surfaceFactory())
  //    , surface(qSharedPointerDynamicCast<Surface>(surface_factory->create(g_surface_options)))
    : q(_q)
#ifdef EXTENDED_SURFACE_TEMP_DISABLED
    , extended_surface(qSharedPointerDynamicCast<Surface>(surface_factory->create(g_extended_surface_options, surface)))
    , magnifier_surface(qSharedPointerDynamicCast<Surface>(surface_factory->create(g_extended_surface_options, surface)))
#endif
    , qmlRootItem(0)
    , editor(EditorOptions(), new Model::Text, new Logic::WordEngine, new Logic::LanguageFeatures)
    , feedback()
    , style(new Style)
    , notifier()
    , key_overrides()
    , settings()
    , layout()
    , extended_layout()
    , magnifier_layout()
    , context(q, style)
    , host(host)
    , view(0)
    , predictionEnabled(false)
    , contentType(Maliit::FreeTextContentType)
    , activeLanguageId("en_us")
    , applicationApiWrapper(new UbuntuapplicationApiWrapper)
{
    view = createWindow(host);

    editor.setHost(host);

    layout.updater.setLayout(&layout.helper);
    extended_layout.updater.setLayout(&extended_layout.helper);

    layout.updater.setStyle(style);
    extended_layout.updater.setStyle(style);
    feedback.setStyle(style);

    const QSize &screen_size(view->screen()->size());
    layout.helper.setScreenSize(screen_size);
    layout.helper.setAlignment(Logic::LayoutHelper::Bottom);
    extended_layout.helper.setScreenSize(screen_size);
    extended_layout.helper.setAlignment(Logic::LayoutHelper::Floating);

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

#ifdef EXTENDED_SURFACE_TEMP_DISABLED
    QObject::connect(&layout.event_handler,          SIGNAL(extendedKeysShown(Key)),
                     &extended_layout.event_handler, SLOT(onExtendedKeysShown(Key)));
#endif
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

#ifdef EXTENDED_SURFACE_TEMP_DISABLED
    QQmlEngine *const extended_engine(extended_surface->view()->engine());
    extended_engine->addImportPath(UBUNTU_KEYBOARD_DATA_DIR);
    setContextProperties(extended_engine->rootContext());

    extended_surface->view()->setSource(QUrl::fromLocalFile(g_maliit_keyboard_extended_qml));

    QQmlEngine *const magnifier_engine(magnifier_surface->view()->engine());
    magnifier_engine->addImportPath(UBUNTU_KEYBOARD_DATA_DIR);
    setContextProperties(magnifier_engine->rootContext());

    magnifier_surface->view()->setSource(QUrl::fromLocalFile(g_maliit_magnifier_qml));
#endif

    // following used to help shell identify the OSK surface
    view->setProperty("role", applicationApiWrapper->oskWindowRole());
    view->setTitle("MaliitOnScreenKeyboard");

    // workaround: resizeMode not working in current qpa imlementation
    // http://qt-project.org/doc/qt-5.0/qtquick/qquickview.html#ResizeMode-enum
    view->setResizeMode(QQuickView::SizeRootObjectToView);
}

void InputMethodPrivate::~InputMethodPrivate()
{
    delete applicationApiWrapper;
}

void InputMethodPrivate::updateWordRibbon()
{
    layout.helper.wordRibbon()->setEnabled( predictionEnabled );
    Q_EMIT q->wordRibbonEnabledChanged( predictionEnabled );
    qmlRootItem->setProperty("wordribbon_visible", predictionEnabled );

    updateKeyboardOrientation();
}

void InputMethodPrivate::setLayoutOrientation(Qt::ScreenOrientation screenOrientation)
{
    Logic::LayoutHelper::Orientation orientation = uiConst->screenToMaliitOrientation(screenOrientation);

    layout.updater.setOrientation(orientation);
    extended_layout.updater.setOrientation(orientation);

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

void InputMethodPrivate::updateKeyboardOrientation()
{
    setLayoutOrientation(QGuiApplication::primaryScreen()->orientation());
}

/*
 * changes keyboard layout
 * called directly to show URL or num layout for special contentTypes,
 * does not change the current language id / activeView
 */

void InputMethodPrivate::setActiveKeyboardId(const QString &id)
{
    // FIXME: Perhaps better to let both LayoutUpdater share the same KeyboardLoader instance?
    layout.updater.setActiveKeyboardId(id);
    extended_layout.updater.setActiveKeyboardId(id);
    layout.model.setActiveView(id);
}

void InputMethodPrivate::connectToNotifier()
{
#ifdef TEMP_DISABLED
    QObject::connect(&notifier, SIGNAL(cursorPositionChanged(int, QString)),
                     &editor,   SLOT(onCursorPositionChanged(int, QString)));
#endif
    QObject::connect(&notifier,      SIGNAL(keysOverriden(Logic::KeyOverrides, bool)),
                     &layout.helper, SLOT(onKeysOverriden(Logic::KeyOverrides, bool)));
}

void InputMethodPrivate::setContextProperties(QQmlContext *qml_context)
{
    qml_context->setContextProperty("maliit", &context);
    qml_context->setContextProperty("maliit_layout", &layout.model);
    qml_context->setContextProperty("maliit_event_handler", &layout.event_handler);
    qml_context->setContextProperty("maliit_extended_layout", &extended_layout.model);
    qml_context->setContextProperty("maliit_extended_event_handler", &extended_layout.event_handler);
    qml_context->setContextProperty("maliit_magnifier_layout", &magnifier_layout);
    qml_context->setContextProperty("maliit_wordribbon", layout.helper.wordRibbon());
}

InputMethod::InputMethod(MAbstractInputMethodHost *host)
    : MAbstractInputMethod(host)
    , d_ptr(new InputMethodPrivate(this, host))
{
    Q_D(InputMethod);

    d->view->setSource(QUrl::fromLocalFile(g_maliit_keyboard_qml));

    // FIXME: Reconnect feedback instance.
    Setup::connectAll(&d->layout.event_handler, &d->layout.updater, &d->editor);
    Setup::connectAll(&d->extended_layout.event_handler, &d->extended_layout.updater, &d->editor);

    connect(&d->layout.helper, SIGNAL(centerPanelChanged(KeyArea,Logic::KeyOverrides)),
            &d->layout.model, SLOT(setKeyArea(KeyArea)));

    connect(&d->extended_layout.helper, SIGNAL(extendedPanelChanged(KeyArea,Logic::KeyOverrides)),
            &d->extended_layout.model, SLOT(setKeyArea(KeyArea)));

    connect(&d->layout.helper,    SIGNAL(magnifierChanged(KeyArea)),
            &d->magnifier_layout, SLOT(setKeyArea(KeyArea)));

#ifdef EXTENDED_SURFACE_TEMP_DISABLED
    connect(&d->layout.model, SIGNAL(widthChanged(int)),
            this,             SLOT(onLayoutWidthChanged(int)));

    connect(&d->layout.model, SIGNAL(heightChanged(int)),
            this,             SLOT(onLayoutHeightChanged(int)));

    connect(&d->layout.updater, SIGNAL(keyboardTitleChanged(QString)),
            &d->layout.model,   SLOT(setTitle(QString)));

    connect(&d->extended_layout.model, SIGNAL(widthChanged(int)),
            this,                      SLOT(onExtendedLayoutWidthChanged(int)));

    connect(&d->extended_layout.model, SIGNAL(heightChanged(int)),
            this,                      SLOT(onExtendedLayoutHeightChanged(int)));

    connect(&d->extended_layout.model, SIGNAL(originChanged(QPoint)),
            this,                      SLOT(onExtendedLayoutOriginChanged(QPoint)));

    connect(&d->magnifier_layout, SIGNAL(widthChanged(int)),
            this,                 SLOT(onMagnifierLayoutWidthChanged(int)));

    connect(&d->magnifier_layout, SIGNAL(heightChanged(int)),
            this,                 SLOT(onMagnifierLayoutHeightChanged(int)));

    connect(&d->magnifier_layout, SIGNAL(originChanged(QPoint)),
            this,                 SLOT(onMagnifierLayoutOriginChanged(QPoint)));
#endif

    connect(&d->editor, SIGNAL(rightLayoutSelected()),
            this,       SLOT(onRightLayoutSelected()));

    connect(this, SIGNAL(wordRibbonEnabledChanged(bool)), uiConst, SLOT(onWordEngineSettingsChanged(bool)));

    connect(this, SIGNAL(predictionEnabledChanged()), this, SLOT(updateWordEngine()));
    connect(this, SIGNAL(contentTypeChanged(Maliit::TextContentType)), this, SLOT(onContentTypeChanged(Maliit::TextContentType)));

    registerStyleSetting(host);

    registerFeedbackSetting(host);
    registerAutoCorrectSetting(host);
    registerAutoCapsSetting(host);
    registerWordEngineSetting(host);

    setActiveSubView("en_us");

    // Setting layout orientation depends on word engine and hide word ribbon
    // settings to be initialized first:
    d->updateKeyboardOrientation();
}

InputMethod::~InputMethod()
{}

void InputMethod::show()
{
    Q_D(InputMethod);

    d->view->setVisible(true);

#ifdef EXTENDED_SURFACE_TEMP_DISABLED
    d->surface->show();
    d->extended_surface->show();
    d->magnifier_surface->show();
#endif
    inputMethodHost()->setScreenRegion(QRegion(d->keyboardVisibleRect));

    QRect rect(d->keyboardVisibleRect);
    rect.moveTop( d->windowGeometryRect.height() - d->keyboardVisibleRect.height() );
    inputMethodHost()->setInputMethodArea(rect, d->view);

    qDebug() << "keyboard is reporting <x y w h>: <"
                << d->keyboardVisibleRect.x()
                << d->keyboardVisibleRect.y()
                << d->keyboardVisibleRect.width()
                << d->keyboardVisibleRect.height()
                << "> to the app manager.";

    d->applicationApiWrapper->reportOSKVisible(
                d->keyboardVisibleRect.x(),
                d->keyboardVisibleRect.y(),
                d->keyboardVisibleRect.width(),
                d->keyboardVisibleRect.height()
                );

    d->qmlRootItem->setProperty("shown", true);
}

void InputMethod::hide()
{
    Q_D(InputMethod);
    d->layout.updater.resetOnKeyboardClosed();
    d->editor.clearPreedit();

    d->view->setVisible(false);
#ifdef EXTENDED_SURFACE_TEMP_DISABLED
    d->surface->hide();
    d->extended_surface->hide();
    d->magnifier_surface->hide();

    const QRegion r;
    inputMethodHost()->setScreenRegion(r);
    inputMethodHost()->setInputMethodArea(r);
#endif

    d->applicationApiWrapper->reportOSKInvisible();

    d->qmlRootItem->setProperty("shown", false);
}

void InputMethod::setPreedit(const QString &preedit,
                             int cursor_position)
{
    Q_UNUSED(cursor_position)
    Q_D(InputMethod);
    d->editor.replacePreedit(preedit);
}

void InputMethod::switchContext(Maliit::SwitchDirection direction,
                                bool animated)
{
    Q_UNUSED(direction)
    Q_UNUSED(animated)
}

QList<MAbstractInputMethod::MInputMethodSubView>
InputMethod::subViews(Maliit::HandlerState state) const
{
    Q_UNUSED(state)
    Q_D(const InputMethod);

    QList<MInputMethodSubView> views;

    Q_FOREACH (const QString &id, d->layout.updater.keyboardIds()) {
        MInputMethodSubView v;
        v.subViewId = id;
        v.subViewTitle = d->layout.updater.keyboardTitle(id);
        views.append(v);
    }

    return views;
}

// called by settings change/language change
void InputMethod::setActiveSubView(const QString &id,
                                   Maliit::HandlerState state)
{
    Q_UNUSED(state)
    Q_D(InputMethod);

    // store language id, so we can switch back to current active view
    // after showing special layouts as e.g. URL or Num layouts
    d->activeLanguageId = id;
    d->setActiveKeyboardId(id);
}

QString InputMethod::activeSubView(Maliit::HandlerState state) const
{
    Q_UNUSED(state)
    Q_D(const InputMethod);

    return d->layout.updater.activeKeyboardId();
}

void InputMethod::handleFocusChange(bool focusIn) {
    if (not focusIn)
        hide();
}

void InputMethod::handleAppOrientationChanged(int angle)
{
    Q_D(InputMethod);

    Qt::ScreenOrientation orientation = Qt::PortraitOrientation;
    switch (angle) {
        case 0:
            orientation = Qt::LandscapeOrientation; break;
        case 90:
            orientation = Qt::InvertedPortraitOrientation; break;
        case 180:
            orientation = Qt::InvertedLandscapeOrientation; break;
        case 270:
        default:
            orientation = Qt::PortraitOrientation; break;
    }

    d->setLayoutOrientation(orientation);
}

bool InputMethod::imExtensionEvent(MImExtensionEvent *event)
{
    Q_D(InputMethod);

    if (not event or event->type() != MImExtensionEvent::Update) {
        return false;
    }

    MImUpdateEvent *update_event(static_cast<MImUpdateEvent *>(event));

    d->notifier.notify(update_event);

    return true;
}

void InputMethod::registerStyleSetting(MAbstractInputMethodHost *host)
{
    Q_D(InputMethod);

    QVariantMap attributes;
    QStringList available_styles = d->style->availableProfiles();
    attributes[Maliit::SettingEntryAttributes::defaultValue] = MALIIT_DEFAULT_PROFILE;
    attributes[Maliit::SettingEntryAttributes::valueDomain] = available_styles;
    attributes[Maliit::SettingEntryAttributes::valueDomainDescriptions] = available_styles;

    d->settings.style.reset(host->registerPluginSetting("current_style",
                                                        QT_TR_NOOP("Keyboard style"),
                                                        Maliit::StringType,
                                                        attributes));

    connect(d->settings.style.data(), SIGNAL(valueChanged()),
            this,                     SLOT(onStyleSettingChanged()));

    // Call manually for the first time to initialize dependent values:
    onStyleSettingChanged();
}

void InputMethod::registerFeedbackSetting(MAbstractInputMethodHost *host)
{
    Q_D(InputMethod);

    QVariantMap attributes;
    attributes[Maliit::SettingEntryAttributes::defaultValue] = true;

    d->settings.feedback.reset(host->registerPluginSetting("feedback_enabled",
                                                           QT_TR_NOOP("Feedback enabled"),
                                                           Maliit::BoolType,
                                                           attributes));

    connect(d->settings.feedback.data(), SIGNAL(valueChanged()),
            this,                        SLOT(onFeedbackSettingChanged()));

    d->feedback.setEnabled(d->settings.feedback->value().toBool());
}

void InputMethod::registerAutoCorrectSetting(MAbstractInputMethodHost *host)
{
    Q_D(InputMethod);

    QVariantMap attributes;
    attributes[Maliit::SettingEntryAttributes::defaultValue] = true;

    d->settings.auto_correct.reset(host->registerPluginSetting("auto_correct_enabled",
                                                               QT_TR_NOOP("Auto-correct enabled"),
                                                               Maliit::BoolType,
                                                               attributes));

    connect(d->settings.auto_correct.data(), SIGNAL(valueChanged()),
            this,                            SLOT(onAutoCorrectSettingChanged()));

    d->editor.setAutoCorrectEnabled(d->settings.auto_correct->value().toBool());
}

void InputMethod::registerAutoCapsSetting(MAbstractInputMethodHost *host)
{
    Q_D(InputMethod);

    QVariantMap attributes;
    attributes[Maliit::SettingEntryAttributes::defaultValue] = true;

    d->settings.auto_caps.reset(host->registerPluginSetting("auto_caps_enabled",
                                                            QT_TR_NOOP("Auto-capitalization enabled"),
                                                            Maliit::BoolType,
                                                            attributes));

    connect(d->settings.auto_caps.data(), SIGNAL(valueChanged()),
            this,                         SLOT(onAutoCapsSettingChanged()));

    d->editor.setAutoCapsEnabled(d->settings.auto_caps->value().toBool());
}

void InputMethod::registerWordEngineSetting(MAbstractInputMethodHost *host)
{
    Q_D(InputMethod);

    QVariantMap attributes;
    attributes[Maliit::SettingEntryAttributes::defaultValue] = false;

    d->settings.word_engine.reset(host->registerPluginSetting("word_engine_enabled",
                                                              QT_TR_NOOP("Error correction/word prediction enabled"),
                                                              Maliit::BoolType,
                                                              attributes));

    connect(d->settings.word_engine.data(), SIGNAL(valueChanged()),
            this,                           SLOT(updateWordEngine()));

    Q_EMIT wordEngineEnabledChanged( d->settings.word_engine.data()->value().toBool() );

#ifndef DISABLE_PREEDIT
    d->editor.wordEngine()->setEnabled(d->settings.word_engine->value().toBool());
#else
    d->editor.wordEngine()->setEnabled(false);
#endif
}

void InputMethod::onLeftLayoutSelected()
{
    // This API smells real bad.
    const QList<MImSubViewDescription> &list =
        inputMethodHost()->surroundingSubViewDescriptions(Maliit::OnScreen);

    if (list.count() > 0) {
        Q_EMIT activeSubViewChanged(list.at(0).id());
    }
}

void InputMethod::onRightLayoutSelected()
{
    // This API smells real bad.
    const QList<MImSubViewDescription> &list =
        inputMethodHost()->surroundingSubViewDescriptions(Maliit::OnScreen);

    if (list.count() > 1) {
        Q_EMIT activeSubViewChanged(list.at(1).id());
    }
}

void InputMethod::onScreenSizeChange(const QSize &size)
{
    Q_D(InputMethod);

    d->layout.helper.setScreenSize(size);
    d->extended_layout.helper.setScreenSize(d->layout.helper.screenSize());

#ifdef TEMP_DISABLED
    d->updateKeyboardOrientation();
#endif
}

void InputMethod::onStyleSettingChanged()
{
    Q_D(InputMethod);
    d->style->setProfile(d->settings.style->value().toString());
    d->layout.model.setImageDirectory(d->style->directory(Style::Images));
    d->extended_layout.model.setImageDirectory(d->style->directory(Style::Images));
    d->magnifier_layout.setImageDirectory(d->style->directory(Style::Images));
}

void InputMethod::onFeedbackSettingChanged()
{
    Q_D(InputMethod);
    d->feedback.setEnabled(d->settings.feedback->value().toBool());
}

void InputMethod::onAutoCorrectSettingChanged()
{
    Q_D(InputMethod);
    d->editor.setAutoCorrectEnabled(d->settings.auto_correct->value().toBool());
}

void InputMethod::onAutoCapsSettingChanged()
{
    Q_D(InputMethod);
    d->editor.setAutoCapsEnabled(d->settings.auto_caps->value().toBool());
}

void InputMethod::setKeyOverrides(const QMap<QString, QSharedPointer<MKeyOverride> > &overrides)
{
    Q_D(InputMethod);

    for (OverridesIterator i(d->key_overrides.begin()), e(d->key_overrides.end()); i != e; ++i) {
        const SharedOverride &override(i.value());

        if (override) {
            disconnect(override.data(), SIGNAL(keyAttributesChanged(const QString &, const MKeyOverride::KeyOverrideAttributes)),
                       this,            SLOT(updateKey(const QString &, const MKeyOverride::KeyOverrideAttributes)));
        }
    }

    d->key_overrides.clear();
    QMap<QString, Key> overriden_keys;

    for (OverridesIterator i(overrides.begin()), e(overrides.end()); i != e; ++i) {
        const SharedOverride &override(i.value());

        if (override) {
            d->key_overrides.insert(i.key(), override);
            connect(override.data(), SIGNAL(keyAttributesChanged(const QString &, const MKeyOverride::KeyOverrideAttributes)),
                    this,            SLOT(updateKey(const QString &, const MKeyOverride::KeyOverrideAttributes)));
            overriden_keys.insert(i.key(), overrideToKey(override));
        }
    }
    d->notifier.notifyOverride(overriden_keys);
}

void InputMethod::updateKey(const QString &key_id,
                            const MKeyOverride::KeyOverrideAttributes changed_attributes)
{
    Q_D(InputMethod);

    Q_UNUSED(changed_attributes);

    QMap<QString, SharedOverride>::iterator iter(d->key_overrides.find(key_id));

    if (iter != d->key_overrides.end()) {
        const Key &override_key(overrideToKey(iter.value()));
        Logic::KeyOverrides overrides_update;

        overrides_update.insert(key_id, override_key);
        d->notifier.notifyOverride(overrides_update, true);
    }
}

void InputMethod::onKeyboardClosed()
{
    hide();
    inputMethodHost()->notifyImInitiatedHiding();
}

void InputMethod::onLayoutWidthChanged(int width)
{
  Q_UNUSED(width);
}

void InputMethod::onLayoutHeightChanged(int height)
{
  Q_UNUSED(height);
}

#ifdef EXTENDED_SURFACE_TEMP_DISABLED
void InputMethod::onExtendedLayoutWidthChanged(int width)
{
    Q_D(InputMethod);
    d->extended_surface->setSize(QSize(width, d->extended_surface->size().height()));
}

void InputMethod::onExtendedLayoutHeightChanged(int height)
{
    Q_D(InputMethod);
    d->extended_surface->setSize(QSize(d->extended_surface->size().width(), height));
}

void InputMethod::onExtendedLayoutOriginChanged(const QPoint &origin)
{
    Q_D(InputMethod);
    d->extended_surface->setRelativePosition(origin);
}

void InputMethod::onMagnifierLayoutWidthChanged(int width)
{
    Q_D(InputMethod);
    d->magnifier_surface->setSize(QSize(width, d->magnifier_surface->size().height()));
}

void InputMethod::onMagnifierLayoutHeightChanged(int height)
{
    Q_D(InputMethod);
    d->magnifier_surface->setSize(QSize(d->magnifier_surface->size().width(), height));
}

void InputMethod::onMagnifierLayoutOriginChanged(const QPoint &origin)
{
    Q_D(InputMethod);
    d->magnifier_surface->setRelativePosition(origin);
}
#endif

void InputMethod::onHideAnimationFinished()
{
    Q_D(InputMethod);

    d->qmlRootItem->setProperty("hidePropertyAnimationFinished", false);

    if (d->qmlRootItem->property("state").toByteArray() == "HIDDEN") {
        d->host->notifyImInitiatedHiding();
        hide();
    }
}

void InputMethod::deviceOrientationChanged(Qt::ScreenOrientation orientation)
{
    Q_UNUSED(orientation);
    Q_D(InputMethod);
    d->updateKeyboardOrientation();
}

void InputMethod::update()
{
    Q_D(InputMethod);

    bool valid;

    bool emitPredictionEnabled = false;
    bool emitContentType = false;

    bool newPredictionEnabled = inputMethodHost()->predictionEnabled(valid);

    if (!valid)
        newPredictionEnabled = true;

    if (d->predictionEnabled != newPredictionEnabled) {
        d->predictionEnabled = newPredictionEnabled;
        emitPredictionEnabled = true;
    }

    Maliit::TextContentType newContentType = static_cast<Maliit::TextContentType>( inputMethodHost()->contentType(valid) );
    if (!valid) {
        newContentType = Maliit::FreeTextContentType;
    }

    if (newContentType != d->contentType) {
        d->contentType = newContentType;
        emitContentType = true;
    }


    if (emitPredictionEnabled)
        Q_EMIT predictionEnabledChanged();

    if (emitContentType)
           Q_EMIT contentTypeChanged(d->contentType);

}

void InputMethod::updateWordEngine()
{
    Q_D(InputMethod);

    if (!d->settings.word_engine.data()->value().toBool())
        d->predictionEnabled = false;

    d->editor.clearPreedit();
    d->editor.wordEngine()->setEnabled( d->predictionEnabled );
    d->updateWordRibbon();
}

bool InputMethod::predictionEnabled()
{
    Q_D(InputMethod);
    return d->predictionEnabled;
}

Maliit::TextContentType InputMethod::contentType()
{
    Q_D(InputMethod);
    return d->contentType;
}

void InputMethod::onContentTypeChanged(Maliit::TextContentType contentType)
{
    Q_D(InputMethod);

    if (contentType == Maliit::FreeTextContentType)
        d->setActiveKeyboardId( d->activeLanguageId );

    if (contentType == Maliit::NumberContentType)
        d->setActiveKeyboardId( "number" );

    if (contentType == Maliit::PhoneNumberContentType)
        d->setActiveKeyboardId( "phonenumber" );

    if (contentType == Maliit::EmailContentType)
        d->setActiveKeyboardId( "email" );

    if (contentType == Maliit::UrlContentType)
        d->setActiveKeyboardId("url");
}

void InputMethod::onQQuickViewStatusChanged(QQuickView::Status status)
{
    Q_D(InputMethod);

    switch (status) {
    case QQuickView::Ready:
    {
        d->qmlRootItem = qobject_cast<QQuickItem*> (d->view->rootObject());
        QObject::connect(d->qmlRootItem, SIGNAL(hideAnimationFinishedChanged()), this, SLOT(onHideAnimationFinished()));
    }
        break;
    default:
        break;
    }
}

} // namespace MaliitKeyboard
