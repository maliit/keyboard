/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 One Laptop per Child Association
 *
 * Contact: maliit-discuss@lists.maliit.org
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

#include "soundfeedback.h"

#include "logic/style.h"
#include "models/styleattributes.h"

#if QT_VERSION < 0x050000
#include <QFeedbackFileEffect>

QTM_USE_NAMESPACE
#else
class QFeedbackFileEffect
{
public:
    void setSource(const QUrl &) {}
    void start() {}
    void stop() {}
};
#endif


//! \class MaliitKeyboard::SoundFeedback
//! Provides a sound-playing feedback backend. Used as default backend if
//! QtMobility is configured and ready to use.

namespace MaliitKeyboard {

enum EffectIndex
{
    KeyPressEffect,
    KeyReleaseEffect,
    LayoutChangeEffect,
    KeyboardHideEffect,

    EffectsCount
};

class SoundFeedbackPrivate
{
public:
    QFeedbackFileEffect effects[EffectsCount];
    SharedStyle style;

    SoundFeedbackPrivate();
    void playEffect(EffectIndex play_index);
    void setupEffect(EffectIndex index,
                     const QString &sounds_dir,
                     const QByteArray &file);
};

SoundFeedbackPrivate::SoundFeedbackPrivate()
    : effects()
    , style()
{}

void SoundFeedbackPrivate::playEffect(EffectIndex play_index)
{
    /* We are iterating over whole array to stop all sounds but one,
     * which will be played.  For now, there is no need to make it
     * sophisticated (separate array of effects being now played or
     * maybe differentiating between interruptible and uninterruptible
     * effects). */
    for (int index(0); index < EffectsCount; ++index) {
        QFeedbackFileEffect& effect(effects[index]);

        if (index == play_index) {
            effect.start();
        } else {
            effect.stop();
        }
    }
}

/* This function is only because we have to check explicitly whether
 * gotten file name is not empty. Otherwise we could get some mutex
 * lock failures somewhere in QtMobility:
 *
 * WARNING: QSoundEffect(pulseaudio): Error decoding source
 * WARNING: QSoundEffect(pulseaudio): Error decoding source
 * WARNING: QMutex::unlock: mutex lock failure: Invalid argument
 */
void SoundFeedbackPrivate::setupEffect(EffectIndex index,
                                       const QString &sounds_dir,
                                       const QByteArray &file)
{
    if (file.isEmpty()) {
        return;
    }

    effects[index].setSource(QUrl::fromLocalFile(sounds_dir + "/" + file));
}

//! @param parent The owner of this instance. Can be 0, in case QObject
//!               ownership is not required.
SoundFeedback::SoundFeedback(QObject *parent)
    : AbstractFeedback(parent)
    , d_ptr(new SoundFeedbackPrivate)
{
    setEnabled(true);
}

SoundFeedback::~SoundFeedback()
{}

void SoundFeedback::setStyle(const SharedStyle &style)
{
    Q_D(SoundFeedback);
    if (d->style != style) {
        if (d->style) {
            disconnect(d->style.data(), SIGNAL(profileChanged()),
                       this,            SLOT(applyProfile()));
        }
        d->style = style;

        if (d->style.isNull()) {
            return;
        }

        connect(d->style.data(), SIGNAL(profileChanged()),
                this,            SLOT(applyProfile()));
        applyProfile();
    }
}

void SoundFeedback::applyProfile()
{
    Q_D(SoundFeedback);
    const QString path(d->style->directory(Style::Sounds));
    const StyleAttributes *attributes(d->style->attributes());

    d->setupEffect(KeyPressEffect, path, attributes->keyPressSound());
    d->setupEffect(KeyReleaseEffect, path, attributes->keyReleaseSound());
    d->setupEffect(LayoutChangeEffect, path, attributes->layoutChangeSound());
    d->setupEffect(KeyboardHideEffect, path, attributes->keyboardHideSound());
}

void SoundFeedback::playPressFeedback()
{
    Q_D(SoundFeedback);
    d->playEffect(KeyPressEffect);
}

void SoundFeedback::playReleaseFeedback()
{
    Q_D(SoundFeedback);
    d->playEffect(KeyReleaseEffect);
}

void SoundFeedback::playLayoutChangeFeedback()
{
    Q_D(SoundFeedback);
    d->playEffect(LayoutChangeEffect);
}

void SoundFeedback::playKeyboardHideFeedback()
{
    Q_D(SoundFeedback);
    d->playEffect(KeyboardHideEffect);
}

} // namespace MaliitKeyboard
