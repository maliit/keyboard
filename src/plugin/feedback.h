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

#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QObject>

#include <memory>

class QSoundEffect;
class QFeedbackHapticsEffect;

namespace MaliitKeyboard
{

class KeyboardSettings;

class Feedback: public QObject
{
Q_OBJECT

    Q_PROPERTY(bool useAudioFeedback READ useAudioFeedback NOTIFY useAudioFeedbackChanged)
    Q_PROPERTY(QString audioFeedbackSound READ audioFeedbackSound NOTIFY audioFeedbackSoundChanged)
    Q_PROPERTY(bool useHapticFeedback READ useHapticFeedback NOTIFY useHapticFeedbackChanged)

public:
    explicit Feedback(const KeyboardSettings *settings);
    ~Feedback() override;

    Q_INVOKABLE void playAudio();
    Q_INVOKABLE void startPressEffect();
    Q_INVOKABLE void keyPressed();

    [[nodiscard]] bool useAudioFeedback() const;
    [[nodiscard]] QString audioFeedbackSound() const;
    [[nodiscard]] bool useHapticFeedback() const;

Q_SIGNALS:
    void useAudioFeedbackChanged(bool);
    void audioFeedbackSoundChanged(QString);
    void useHapticFeedbackChanged(bool);

private:
    const KeyboardSettings *m_settings;
    std::unique_ptr<QSoundEffect> m_audioEffect;
#ifdef HAVE_QT5_FEEDBACK
    std::unique_ptr<QFeedbackHapticsEffect> m_pressEffect;
#endif
};

}

#endif //FEEDBACK_H
