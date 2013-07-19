// -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil; c-file-offsets: ((innamespace . 0)); -*-
/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: Mohammad Anwari <Mohammad.Anwari@nokia.com>
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

#ifndef MALIIT_KEYBOARD_KEYBOARDLOADER_H
#define MALIIT_KEYBOARD_KEYBOARDLOADER_H

#include "models/key.h"
#include "models/keyboard.h"

#include <QtCore>

namespace MaliitKeyboard {

class KeyboardLoaderPrivate;

class KeyboardLoader
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(KeyboardLoader)
    Q_DECLARE_PRIVATE(KeyboardLoader)

public:
    explicit KeyboardLoader(QObject *parent = 0);
    virtual ~KeyboardLoader();

    virtual QStringList ids() const;
    virtual QString activeId() const;
    virtual void setActiveId(const QString &id);

    virtual QString title(const QString &id) const;

    virtual Keyboard keyboard() const;
    virtual Keyboard nextKeyboard() const;
    virtual Keyboard previousKeyboard() const;

    virtual Keyboard shiftedKeyboard() const;
    virtual Keyboard symbolsKeyboard(int page = 0) const;
    virtual Keyboard deadKeyboard(const Key &dead) const;
    virtual Keyboard shiftedDeadKeyboard(const Key &dead) const;
    virtual Keyboard extendedKeyboard(const Key &key) const;
    virtual Keyboard numberKeyboard() const;
    virtual Keyboard phoneNumberKeyboard() const;

    Q_SIGNAL void keyboardsChanged() const;

private:
    const QScopedPointer<KeyboardLoaderPrivate> d_ptr;
};

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_KEYBOARDLOADER_H
