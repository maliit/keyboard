/*
 * Copyright 2019 Jan Arne Petersen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "feedback.h"

#include "keyboardsettings.h"

#include <QtMultimedia/QSoundEffect>
#include <QtFeedback/QFeedbackHapticsEffect>

#include <memory>

namespace MaliitKeyboard {

Feedback::Feedback(const KeyboardSettings *settings)
    : QObject()
    , m_settings(settings)
    , m_audioEffect(std::make_unique<QSoundEffect>())
    , m_pressEffect(std::make_unique<QFeedbackHapticsEffect>())
{
    connect(settings, &KeyboardSettings::keyPressAudioFeedbackChanged, this, &Feedback::useAudioFeedbackChanged);
    connect(settings, &KeyboardSettings::keyPressAudioFeedbackSoundChanged, this, &Feedback::audioFeedbackSoundChanged);
    connect(settings, &KeyboardSettings::keyPressHapticFeedbackChanged, this, &Feedback::useHapticFeedbackChanged);
    m_audioEffect->setSource(QUrl(audioFeedbackSound()));
    m_pressEffect->setAttackIntensity(0.0);
    m_pressEffect->setAttackTime(50);
    m_pressEffect->setIntensity(1.0);
    m_pressEffect->setDuration(10);
    m_pressEffect->setFadeTime(50);
    m_pressEffect->setFadeIntensity(0.0);
}

Feedback::~Feedback() = default;

void Feedback::playAudio()
{
    if (useAudioFeedback())
        m_audioEffect->play();
}

void Feedback::startPressEffect()
{
    if (useHapticFeedback())
        m_pressEffect->start();
}

void Feedback::keyPressed()
{
    playAudio();
    startPressEffect();
}

bool Feedback::useAudioFeedback() const
{
    return m_settings->keyPressAudioFeedback();
}

QString Feedback::audioFeedbackSound() const
{
    return m_settings->keyPressAudioFeedbackSound();
}

bool Feedback::useHapticFeedback() const
{
    return m_settings->keyPressHapticFeedback();
}

}