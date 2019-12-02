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
    std::unique_ptr<QFeedbackHapticsEffect> m_pressEffect;
};

}

#endif //FEEDBACK_H
