/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 Openismus GmbH. All rights reserved.
 *
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

#ifndef MALIIT_KEYBOARD_UPDATENOTIFIER_H
#define MALIIT_KEYBOARD_UPDATENOTIFIER_H

#include <QtCore>
#include <maliit/plugins/keyoverride.h>

class MImUpdateEvent;

namespace MaliitKeyboard {
class Key;

namespace Logic
{
typedef QMap<QString, Key> KeyOverrides;
}

class UpdateNotifierPrivate;

class UpdateNotifier
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(UpdateNotifier)
    Q_DECLARE_PRIVATE(UpdateNotifier)

public:
    explicit UpdateNotifier(QObject *parent = nullptr);
    ~UpdateNotifier() override;

    void notify(MImUpdateEvent *event);
    void notifyOverride(const Logic::KeyOverrides &overridden_keys,
                        bool update = false);

    Q_SIGNAL void cursorPositionChanged(int cursor_position,
                                        const QString &surrounding_text);
    Q_SIGNAL void keysOverridden(const Logic::KeyOverrides &overridden_keys,
                                bool update);

private:
    const QScopedPointer<UpdateNotifierPrivate> d_ptr;
};

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_UPDATENOTIFIER_H
