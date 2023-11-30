/*
 * Copyright (c) 2019 Jan Arne Petersen
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

#include "feedback.h"

#include "keyboardsettings.h"

#include <QtMultimedia/QSoundEffect>
#ifdef HAVE_QT5_FEEDBACK
#include <QtFeedback/QFeedbackHapticsEffect>
#endif

#include <memory>

namespace MaliitKeyboard {

Feedback::Feedback(const KeyboardSettings *settings)
    : QObject()
    , m_settings(settings)
    , m_audioEffect(std::make_unique<QSoundEffect>())
#ifdef HAVE_QT5_FEEDBACK
    , m_pressEffect(std::make_unique<QFeedbackHapticsEffect>())
#endif
{
    connect(settings, &KeyboardSettings::keyPressAudioFeedbackChanged, this, &Feedback::useAudioFeedbackChanged);
    connect(settings, &KeyboardSettings::keyPressAudioFeedbackSoundChanged, this, &Feedback::audioFeedbackSoundChanged);
    connect(settings, &KeyboardSettings::keyPressHapticFeedbackChanged, this, &Feedback::useHapticFeedbackChanged);
    m_audioEffect->setSource(QUrl::fromLocalFile(audioFeedbackSound()));
    m_audioEffect->setVolume(0.1);
#ifdef HAVE_QT5_FEEDBACK
    m_pressEffect->setAttackIntensity(0.0);
    m_pressEffect->setAttackTime(50);
    m_pressEffect->setIntensity(0.5);
    m_pressEffect->setDuration(10);
    m_pressEffect->setFadeTime(50);
    m_pressEffect->setFadeIntensity(0.0);
#endif
}

Feedback::~Feedback() = default;

void Feedback::playAudio()
{
    if (useAudioFeedback())
        m_audioEffect->play();
}

void Feedback::startPressEffect()
{
#ifdef HAVE_QT5_FEEDBACK
    if (useHapticFeedback())
        m_pressEffect->start();
#endif
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
