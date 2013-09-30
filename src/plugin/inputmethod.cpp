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
#include "inputmethod_p.h"

#include "models/key.h"
#include "models/keyarea.h"
#include "models/wordribbon.h"
#include "models/layout.h"

#include "logic/layouthelper.h"

#include "logic/style.h"

#include "logic/keyareaconverter.h"

#include "view/setup.h"

#include <maliit/plugins/subviewdescription.h>

#include <maliit/plugins/updateevent.h>

#include <maliit/namespace.h>

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>


class MImUpdateEvent;

namespace MaliitKeyboard {

namespace {

const QString g_maliit_keyboard_qml(UBUNTU_KEYBOARD_DATA_DIR "/Keyboard.qml");

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

InputMethod::InputMethod(MAbstractInputMethodHost *host)
    : MAbstractInputMethod(host)
    , d_ptr(new InputMethodPrivate(this, host))
{
    Q_D(InputMethod);

    d->view->setSource(QUrl::fromLocalFile(g_maliit_keyboard_qml));
    d->view->setGeometry(qGuiApp->primaryScreen()->geometry());

    // FIXME: Reconnect feedback instance.
    Setup::connectAll(&d->layout.event_handler, &d->layout.updater, &d->editor);

    connect(&d->layout.helper, SIGNAL(centerPanelChanged(KeyArea,Logic::KeyOverrides)),
            &d->layout.model, SLOT(setKeyArea(KeyArea)));

    connect(&d->editor,  SIGNAL(autoCapsActivated()), this, SLOT(onAutoCapsActivated()));

    connect(this, SIGNAL(wordRibbonEnabledChanged(bool)), uiConst, SLOT(onWordEngineSettingsChanged(bool)));

    connect(this, SIGNAL(predictionEnabledChanged()), this, SLOT(updateWordEngine()));
    connect(this, SIGNAL(contentTypeChanged(Maliit::TextContentType)), this, SLOT(onContentTypeChanged(Maliit::TextContentType)));

    d->registerStyleSetting(host);

    d->registerFeedbackSetting(host);
    d->registerAutoCorrectSetting(host);
    d->registerAutoCapsSetting(host);
    d->registerWordEngineSetting(host);

    setActiveSubView("en_us");

    // Setting layout orientation depends on word engine and hide word ribbon
    // settings to be initialized first:
    d->setLayoutOrientation(d->appsCurrentOrientation);
}

InputMethod::~InputMethod()
{}

void InputMethod::show()
{
    Q_D(InputMethod);

    d->view->setVisible(true);

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
    d->closeOskWindow();
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

void InputMethod::handleFocusChange(bool focusIn)
{
    if (not focusIn)
        hide();
}

void InputMethod::handleAppOrientationChanged(int angle)
{
    Q_D(InputMethod);

    switch (angle) {
        case 0:
            d->appsCurrentOrientation = Qt::LandscapeOrientation; break;
    case 90:
            d->appsCurrentOrientation = Qt::InvertedPortraitOrientation; break;
        case 180:
            d->appsCurrentOrientation = Qt::InvertedLandscapeOrientation; break;
        case 270:
        default:
            d->appsCurrentOrientation = Qt::PortraitOrientation; break;
    }

    d->setLayoutOrientation(d->appsCurrentOrientation);
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

void InputMethod::onStyleSettingChanged()
{
    Q_D(InputMethod);
    d->style->setProfile(d->settings.style->value().toString());
    d->layout.model.setImageDirectory(d->style->directory(Style::Images));
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

void InputMethod::deviceOrientationChanged(Qt::ScreenOrientation orientation)
{
    Q_UNUSED(orientation);
    Q_D(InputMethod);

    d->setLayoutOrientation(d->appsCurrentOrientation);
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

    if (d->contentType != Maliit::FreeTextContentType)
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

    // TODO when refactoring, forward the enum to QML

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

    updateWordEngine();
}

void InputMethod::onQMLStateChanged(QString state)
{
    Q_D(InputMethod);

    if (state == "HIDDEN")
        d->closeOskWindow();
}

void InputMethod::onQQuickViewStatusChanged(QQuickView::Status status)
{
    Q_D(InputMethod);

    switch (status) {
    case QQuickView::Ready:
    {
        d->qmlRootItem = d->view->rootObject()->findChild<QQuickItem*>("ubuntuKeyboard");
        QObject::connect(d->qmlRootItem, SIGNAL(stateChanged(QString)), this, SLOT(onQMLStateChanged(QString)));
    }
        break;
    default:
        break;
    }
}

/*
 * activated by the editor after e.g. pressing period
 **/

void InputMethod::onAutoCapsActivated()
{
    Q_D(InputMethod);
    d->qmlRootItem->setProperty("autoCapsActivated", true);
}

} // namespace MaliitKeyboard
