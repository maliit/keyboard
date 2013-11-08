/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2013 Canonical, Ltd.
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

#include "keyboardsettings.h"

#include <QDebug>
#include <QGSettings/QGSettings>

using namespace MaliitKeyboard;

const QLatin1String ENABLED_LANGUAGES_KEY = QLatin1String("enabledLanguages");
const QLatin1String AUTO_CAPITALIZATION_KEY = QLatin1String("autoCapitalization");
const QLatin1String AUTO_COMPLETION_KEY = QLatin1String("autoCompletion");
const QLatin1String PREDICTIVE_TEXT_KEY = QLatin1String("predictiveText");
const QLatin1String KEY_PRESS_FEEDBACK_KEY = QLatin1String("keyPressFeedback");

/*!
 * \brief KeyboardSettings::KeyboardSettings class to load the settings, and
 * listens on runtime to changes of them
 * \param parent
 */
KeyboardSettings::KeyboardSettings(QObject *parent) :
    QObject(parent)
  , m_settings(new QGSettings("com.canonical.keyboard.maliit",
                              "/com/canonical/keyboard/maliit/", this))
{
    QObject::connect(m_settings, SIGNAL(changed(QString)),
                     this, SLOT(settingUpdated(QString)));
}

/*!
 * \brief KeyboardSettings::enabledLanguages returns a list of languages that are
 * active
 * \return
 */
QStringList KeyboardSettings::enabledLanguages() const
{
    return m_settings->get(ENABLED_LANGUAGES_KEY).toStringList();
}

/*!
 * \brief KeyboardSettings::autoCapitalization returns true id the first letter
 * of each sentence should be capitalized
 * \return
 */
bool KeyboardSettings::autoCapitalization() const
{
    return m_settings->get(AUTO_CAPITALIZATION_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::autoCompletion returns true if the current word should
 * be completed with first suggestion when hitting space
 * \return
 */
bool KeyboardSettings::autoCompletion() const
{
    return m_settings->get(AUTO_COMPLETION_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::predictiveText returns true, if potential words in the
 * word ribbon should be suggested
 * \return
 */
bool KeyboardSettings::predictiveText() const
{
    return m_settings->get(PREDICTIVE_TEXT_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::keyPressFeedback returns true if feedback is enabled
 * when the user presses a keyboad key
 * \return
 */
bool KeyboardSettings::keyPressFeedback() const
{
    return m_settings->get(KEY_PRESS_FEEDBACK_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::settingUpdated slot to handle changes in the settings backend
 * A specialized signal is emitted for the affected setting
 * \param key
 */
void KeyboardSettings::settingUpdated(const QString &key)
{
    if (key == ENABLED_LANGUAGES_KEY) {
        Q_EMIT enabledLanguagesChanged();
        return;
    } else if (key == AUTO_CAPITALIZATION_KEY) {
        Q_EMIT autoCapitalizationChanged();
        return;
    } else if (key == AUTO_COMPLETION_KEY) {
        Q_EMIT autoCompletionChanged();
        return;
    } else if (key == PREDICTIVE_TEXT_KEY) {
        Q_EMIT predictiveTextChanged();
        return;
    } else if (key == KEY_PRESS_FEEDBACK_KEY) {
        Q_EMIT keyPressFeedbackChanged();
        return;
    }

    qWarning() << Q_FUNC_INFO << "unknown settings key:" << key;
    return;
}
