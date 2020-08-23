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

const QLatin1String ACTIVE_LANGUAGE_KEY = QLatin1String("activeLanguage");
const QLatin1String PREVIOUS_LANGUAGE_KEY = QLatin1String("previousLanguage");
const QLatin1String ENABLED_LANGUAGES_KEY = QLatin1String("enabledLanguages");
const QLatin1String AUTO_CAPITALIZATION_KEY = QLatin1String("autoCapitalization");
const QLatin1String AUTO_COMPLETION_KEY = QLatin1String("autoCompletion");
const QLatin1String PREDICTIVE_TEXT_KEY = QLatin1String("predictiveText");
const QLatin1String SPELL_CHECKING_KEY = QLatin1String("spellChecking");
const QLatin1String KEY_PRESS_AUDIO_FEEDBACK_KEY = QLatin1String("keyPressFeedback");
const QLatin1String KEY_PRESS_AUDIO_FEEDBACK_SOUND_KEY = QLatin1String("keyPressFeedbackSound");
const QLatin1String KEY_PRESS_HAPTIC_FEEDBACK_KEY = QLatin1String("keyPressHapticFeedback");
const QLatin1String DOUBLE_SPACE_FULL_STOP_KEY = QLatin1String("doubleSpaceFullStop");
const QLatin1String STAY_HIDDEN_KEY = QLatin1String("stayHidden");
const QLatin1String DISABLE_HEIGHT_KEY = QLatin1String("disableHeight");
const QLatin1String PLUGIN_PATHS_KEY = QLatin1String("pluginPaths");
const QLatin1String OPACITY_KEY = QLatin1String("opacity");

/*!
 * \brief KeyboardSettings::KeyboardSettings class to load the settings, and
 * listens on runtime to changes of them
 * \param parent
 */
KeyboardSettings::KeyboardSettings(QObject *parent) :
    QObject(parent)
  , m_settings(new QGSettings("org.maliit.keyboard.maliit",
                              "/org/maliit/keyboard/maliit/", this))
{
    QObject::connect(m_settings, &QGSettings::changed,
                     this, &KeyboardSettings::settingUpdated);
}

/*!
 * \brief KeyboardSettings::activeLanguage returns currently active language
 * \return active language
 */

QString KeyboardSettings::activeLanguage() const
{
    return m_settings->get(ACTIVE_LANGUAGE_KEY).toString();
}

void KeyboardSettings::setActiveLanguage(const QString& id)
{
    m_settings->set(ACTIVE_LANGUAGE_KEY, QVariant(id));
}

/*!
 * \brief KeyboardSettings::previousLanguage returns the language that was 
 * active immediately before the current one.
 * \return previously language
 */

QString KeyboardSettings::previousLanguage() const
{
    return m_settings->get(PREVIOUS_LANGUAGE_KEY).toString();
}

void KeyboardSettings::setPreviousLanguage(const QString& id)
{
    m_settings->set(PREVIOUS_LANGUAGE_KEY, QVariant(id));
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
 * \brief KeyboardSettings::spellchecking returns true if spellchecking should be used
 * \return
 */
bool KeyboardSettings::spellchecking() const
{
    return m_settings->get(SPELL_CHECKING_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::keyPressAudioFeedback returns true if audio feedback is enabled
 * when the user presses a keyboad key
 * \return
 */
bool KeyboardSettings::keyPressAudioFeedback() const
{
    return m_settings->get(KEY_PRESS_AUDIO_FEEDBACK_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::keyPressHapticFeedback returns true if haptic feedback is enabled
 * when the user presses a keyboad key
 * \return
 */
bool KeyboardSettings::keyPressHapticFeedback() const
{
    return m_settings->get(KEY_PRESS_HAPTIC_FEEDBACK_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::keyPressFeedbackSound returns the path to the current key
 * feedback sound
 * \return path to the feedback sound
 */
QString KeyboardSettings::keyPressAudioFeedbackSound() const
{
    return m_settings->get(KEY_PRESS_AUDIO_FEEDBACK_SOUND_KEY).toString();
}

/*!
 * \brief KeyboardSettings:doubleSpaceFullStop returns true if double space full-stops are
 * enabled, which insert a full-stop when the space key is pressed twice.
 */
bool KeyboardSettings::doubleSpaceFullStop() const
{
    return m_settings->get(DOUBLE_SPACE_FULL_STOP_KEY).toBool();
}

/*!
 * \brief KeyboardSettings:stayHidden returns true if the keyboard should
 * always remain hidden (e.g. if a hardware keyboard has been connected).
 */
bool KeyboardSettings::stayHidden() const
{
    return m_settings->get(STAY_HIDDEN_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::pluginPaths returns a list of paths containing
 * ubuntu-keyboard layout plugins
 */
QStringList KeyboardSettings::pluginPaths() const
{
    return m_settings->get(PLUGIN_PATHS_KEY).toStringList();
}

bool KeyboardSettings::disableHeight() const
{
    return m_settings->get(DISABLE_HEIGHT_KEY).toBool();
}

/*!
 * \brief KeyboardSettings::opacity returns the transparency value for the
 * keyboard between 0 and 1
 */
double KeyboardSettings::opacity() const
{
    return m_settings->get(OPACITY_KEY).toDouble();
}

/*!
 * \brief KeyboardSettings::settingUpdated slot to handle changes in the settings backend
 * A specialized signal is emitted for the affected setting
 * \param key
 */
void KeyboardSettings::settingUpdated(const QString &key)
{
    if (key == ACTIVE_LANGUAGE_KEY) {
        Q_EMIT activeLanguageChanged(activeLanguage());
        return;
    } else if (key == PREVIOUS_LANGUAGE_KEY) {
        Q_EMIT previousLanguageChanged(previousLanguage());
        return;
    } else if (key == ENABLED_LANGUAGES_KEY) {
        Q_EMIT enabledLanguagesChanged(enabledLanguages());
        return;
    } else if (key == AUTO_CAPITALIZATION_KEY) {
        Q_EMIT autoCapitalizationChanged(autoCapitalization());
        return;
    } else if (key == AUTO_COMPLETION_KEY) {
        Q_EMIT autoCompletionChanged(autoCompletion());
        return;
    } else if (key == PREDICTIVE_TEXT_KEY) {
        Q_EMIT predictiveTextChanged(predictiveText());
        return;
    } else if (key == SPELL_CHECKING_KEY) {
        Q_EMIT spellCheckingChanged(spellchecking());
        return;
    } else if (key == KEY_PRESS_AUDIO_FEEDBACK_KEY) {
        Q_EMIT keyPressAudioFeedbackChanged(keyPressAudioFeedback());
        return;
    } else if (key == KEY_PRESS_HAPTIC_FEEDBACK_KEY) {
        Q_EMIT keyPressHapticFeedbackChanged(keyPressHapticFeedback());
        return;
    } else if (key == KEY_PRESS_AUDIO_FEEDBACK_SOUND_KEY) {
        Q_EMIT keyPressAudioFeedbackSoundChanged(keyPressAudioFeedbackSound());
        return;
    } else if (key == DOUBLE_SPACE_FULL_STOP_KEY) {
        Q_EMIT doubleSpaceFullStopChanged(doubleSpaceFullStop());
        return;
    } else if (key == STAY_HIDDEN_KEY) {
        Q_EMIT stayHiddenChanged(stayHidden());
        return;
    } else if (key == DISABLE_HEIGHT_KEY) {
        Q_EMIT disableHeightChanged(disableHeight());
        return;
    } else if (key == PLUGIN_PATHS_KEY) {
        Q_EMIT pluginPathsChanged(pluginPaths());
    } else if (key == OPACITY_KEY) {
        Q_EMIT opacityChanged(opacity());
    } else if (key == "theme")
        Q_EMIT themeChanged(theme());

    qWarning() << Q_FUNC_INFO << "unknown settings key:" << key;
}

QString KeyboardSettings::theme() const
{
    return m_settings->get("theme").toString();
}
