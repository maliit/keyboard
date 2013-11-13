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

#include "view/setup.h"

#include <maliit/plugins/subviewdescription.h>
#include <maliit/plugins/updateevent.h>
#include <maliit/namespace.h>

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>


class MImUpdateEvent;

using namespace MaliitKeyboard;

namespace {

const QString g_maliit_keyboard_qml(UBUNTU_KEYBOARD_DATA_DIR "/Keyboard.qml");

Key overrideToKey(const SharedOverride &override)
{
    Key key;

    key.rLabel() = override->label();
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

    connect(&d->editor,  SIGNAL(autoCapsActivated()), this, SIGNAL(activateAutocaps()));

    connect(this, SIGNAL(wordRibbonEnabledChanged(bool)), uiConst, SLOT(onWordEngineSettingsChanged(bool)));

    connect(this, SIGNAL(predictionEnabledChanged()), this, SLOT(updateWordEngine()));
    connect(this, SIGNAL(contentTypeChanged(TextContentType)), this, SLOT(setContentType(TextContentType)));
    connect(this, SIGNAL(activeLanguageChanged(QString)), d->editor.wordEngine(), SLOT(onLanguageChanged(QString)));
    connect(d->m_geometry, SIGNAL(visibleRectChanged()), this, SLOT(onVisibleRectChanged()));

    d->registerStyleSetting(host);

    d->registerFeedbackSetting();
    d->registerAutoCorrectSetting();
    d->registerAutoCapsSetting();
    d->registerWordEngineSetting();
    d->registerEnabledLanguages();

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
    d->m_geometry->setShown(true);
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
    Q_UNUSED(id);
    Q_D(InputMethod);

    // FIXME: Perhaps better to let both LayoutUpdater share the same KeyboardLoader instance?
    d->layout.updater.setActiveKeyboardId(id);
    d->layout.model.setActiveView(id);

    d->registerSystemLanguage();
    setActiveLanguage(d->systemLanguage);
}

QString InputMethod::activeSubView(Maliit::HandlerState state) const
{
    Q_UNUSED(state)
    Q_D(const InputMethod);

    return d->layout.updater.activeKeyboardId();
}

void InputMethod::handleFocusChange(bool focusIn)
{
    if (focusIn) {
        checkInitialAutocaps();
    } else {
        hide();
    }
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
    d->feedback.setEnabled(d->m_settings.keyPressFeedback());
}

void InputMethod::onAutoCorrectSettingChanged()
{
    Q_D(InputMethod);
    d->editor.setAutoCorrectEnabled(d->m_settings.autoCompletion());
}

/*!
 * \brief InputMethod::updateAutoCaps enabled the use of auto capitalization
 * when the setting iss eto true, and the text area does not prevent to use it
 */
void InputMethod::updateAutoCaps()
{
    Q_D(InputMethod);
    bool enabled = d->m_settings.autoCapitalization();
    enabled &= d->contentType == FreeTextContentType;
    bool valid = true;
    bool autocap = d->host->autoCapitalizationEnabled(valid);
    enabled &= autocap;

    if (enabled != d->autocapsEnabled) {
        d->autocapsEnabled = enabled;
        d->editor.setAutoCapsEnabled(enabled);
    }
}

//! \brief InputMethod::onEnabledLanguageSettingsChanged
//! Updates the list of languages that can be used
void InputMethod::onEnabledLanguageSettingsChanged()
{
    Q_D(InputMethod);
    d->truncateEnabledLanguageLocales(d->m_settings.enabledLanguages());
    Q_EMIT enabledLanguagesChanged(d->enabledLanguages);
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

    bool newPredictionEnabled = inputMethodHost()->predictionEnabled(valid);

    if (!valid)
        newPredictionEnabled = true;

    if (d->predictionEnabled != newPredictionEnabled) {
        d->predictionEnabled = newPredictionEnabled;
        emitPredictionEnabled = true;
    }

    TextContentType newContentType = static_cast<TextContentType>( inputMethodHost()->contentType(valid) );
    if (!valid) {
        newContentType = FreeTextContentType;
    }
    setContentType(newContentType);

    if (emitPredictionEnabled) {
        d->updateWordRibbon();
        Q_EMIT predictionEnabledChanged();
    }

    QString text;
    int position;
    bool ok = d->host->surroundingText(text, position);
    if (ok) {
        d->editor.text()->setSurrounding(text);
        d->editor.text()->setSurroundingOffset(position);
    }

    updateAutoCaps();
}

void InputMethod::updateWordEngine()
{
    Q_D(InputMethod);

    if (!d->m_settings.predictiveText())
        d->predictionEnabled = false;

    if (d->contentType != FreeTextContentType)
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

//! \brief InputMethod::showWordRibbon returns if the word ribbon should be shown
bool InputMethod::showWordRibbon()
{
    Q_D(InputMethod);
    return d->showWordRibbon;
}

//! \brief InputMethod::contentType returns the type, of the input field, like free text, email, url
//! \return
InputMethod::TextContentType InputMethod::contentType()
{
    Q_D(InputMethod);
    return d->contentType;
}

//! \brief InputMethod::setContentType sets the type, of the input field, like free text, email, url
//! \param contentType
void InputMethod::setContentType(TextContentType contentType)
{
    Q_D(InputMethod);

    if (contentType == d->contentType)
        return;

    setActiveLanguage(d->systemLanguage);

    d->contentType = contentType;
    Q_EMIT contentTypeChanged(contentType);

    updateWordEngine();
    updateAutoCaps();
}

//! \brief InputMethod::checkInitialAutocaps  Checks if the keyboard should be
//! set to uppercase, because the auto caps is enabled and the text is empty.
void InputMethod::checkInitialAutocaps()
{
    Q_D(InputMethod);
    update();

    if (d->autocapsEnabled) {
        QString text;
        int position;
        bool ok = d->host->surroundingText(text, position);
        if (ok && text.isEmpty() && position == 0)
            Q_EMIT activateAutocaps();
    }
}

//! \brief InputMethod::enabledLanguages list of all languages that can be selected
const QStringList &InputMethod::enabledLanguages() const
{
    Q_D(const InputMethod);
    return d->enabledLanguages;
}

//! \brief InputMethod::activeLanguage returns the language that is currently
//! used by the keyboard
const QString &InputMethod::activeLanguage() const
{
    Q_D(const InputMethod);
    return d->activeLanguage;
}

//! \brief InputMethod::systemLanguage returns the languageset as the one used
//! in the whole system
const QString &InputMethod::systemLanguage() const
{
    Q_D(const InputMethod);
    return d->systemLanguage;
}

//! \brief InputMethod::setActiveLanguage
//! Sets the currently active/used language
//! \param newLanguage id of the new language. For example "en" or "es"
//! FIXME check if the language is supported - if not use "en" as fallback
void InputMethod::setActiveLanguage(const QString &newLanguage)
{
    Q_D(InputMethod);

    if (newLanguage.length() != 2) {
        qWarning() << Q_FUNC_INFO << "newLanguage is not valid:" << newLanguage;
        return;
    }

    if (d->activeLanguage == newLanguage)
        return;

    d->activeLanguage = newLanguage;
    d->editor.onLanguageChanged(d->activeLanguage);
    Q_EMIT activeLanguageChanged(d->activeLanguage);
}


QRect InputMethod::windowGeometryRect() const
{
    Q_D(const InputMethod);
    return d->windowGeometryRect;
}

void InputMethod::setWindowGeometryRect(QRect rect)
{
    Q_D(InputMethod);
    d->windowGeometryRect = rect;

    Q_EMIT windowGeometryRectChanged(rect);
}

void InputMethod::onVisibleRectChanged()
{
    Q_D(InputMethod);

    QRect visibleRect = qGuiApp->primaryScreen()->mapBetween(
                            d->m_geometry->orientation(),
                            qGuiApp->primaryScreen()->primaryOrientation(),
                            d->m_geometry->visibleRect().toRect());

    inputMethodHost()->setScreenRegion(QRegion(visibleRect));

    QRect rect(visibleRect);
    rect.moveTop( d->windowGeometryRect.height() - visibleRect.height() );
    inputMethodHost()->setInputMethodArea(rect, d->view);

    qDebug() << "keyboard is reporting <x y w h>: <"
                << visibleRect.x()
                << visibleRect.y()
                << visibleRect.width()
                << visibleRect.height()
                << "> to the app manager.";

    d->applicationApiWrapper->reportOSKVisible(
                visibleRect.x(),
                visibleRect.y(),
                visibleRect.width(),
                visibleRect.height()
                );
}
