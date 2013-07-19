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

#ifndef MALIIT_KEYBOARD_ABSTRACTFEEDBACK_H
#define MALIIT_KEYBOARD_ABSTRACTFEEDBACK_H

#include "logic/style.h"
#include <QtCore>

namespace MaliitKeyboard {

class AbstractFeedbackPrivate;

class AbstractFeedback
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractFeedback)
    Q_DECLARE_PRIVATE(AbstractFeedback)
    Q_PROPERTY(bool enabled READ isEnabled
                            WRITE setEnabled
                            NOTIFY enabledChanged)

public:
    explicit AbstractFeedback(QObject *parent = 0);
    virtual ~AbstractFeedback() = 0;

    virtual void setStyle(const SharedStyle &style) = 0;

    Q_SLOT void setEnabled(bool enabled);
    bool isEnabled() const;
    Q_SIGNAL void enabledChanged(bool enabled);

    Q_SLOT void onKeyPressed();
    Q_SLOT void onKeyReleased();
    Q_SLOT void onLayoutChanged();
    Q_SLOT void onKeyboardHidden();
    // TODO: add slot for key-to-key slide feedback
    // TODO: add slot for keyboard show feedback

private:
    virtual void playPressFeedback() = 0;
    virtual void playReleaseFeedback() = 0;
    virtual void playLayoutChangeFeedback() = 0;
    virtual void playKeyboardHideFeedback() = 0;

    const QScopedPointer<AbstractFeedbackPrivate> d_ptr;
};

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_ABSTRACTFEEDBACK_H
