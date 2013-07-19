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

#ifndef MALIIT_KEYBOARD_SOUNDFEEDBACK_H
#define MALIIT_KEYBOARD_SOUNDFEEDBACK_H

#include "abstractfeedback.h"

namespace MaliitKeyboard {

class SoundFeedbackPrivate;

class SoundFeedback
    : public AbstractFeedback
{
    Q_OBJECT
    Q_DISABLE_COPY(SoundFeedback)
    Q_DECLARE_PRIVATE(SoundFeedback)

public:
    explicit SoundFeedback(QObject *parent = 0);
    virtual ~SoundFeedback();

    virtual void setStyle(const SharedStyle &style);

private:
    const QScopedPointer<SoundFeedbackPrivate> d_ptr;

    virtual void playPressFeedback();
    virtual void playReleaseFeedback();
    virtual void playLayoutChangeFeedback();
    virtual void playKeyboardHideFeedback();

    Q_SLOT void applyProfile();
};

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_SOUNDFEEDBACK_H
