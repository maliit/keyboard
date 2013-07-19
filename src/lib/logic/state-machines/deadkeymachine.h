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

#ifndef MALIIT_KEYBOARD_DEADKEYMACHINE_H
#define MALIIT_KEYBOARD_DEADKEYMACHINE_H

#include "abstractstatemachine.h"
#include "models/key.h"

#include <QtCore>

namespace MaliitKeyboard {
namespace Logic {

class DeadkeyMachinePrivate;

class DeadkeyMachine
    : public QStateMachine
    , public AbstractStateMachine
{
    Q_OBJECT
    Q_DISABLE_COPY(DeadkeyMachine)
    Q_DECLARE_PRIVATE(DeadkeyMachine)

public:
    explicit DeadkeyMachine(QObject *parent = 0);
    virtual ~DeadkeyMachine();

    virtual void setup(LayoutUpdater *updater);

    virtual void setAccentKey(const Key &accent_key);
    Key accentKey() const;

    //! This state means that deadkey wasn't pressed. No accented
    //! characters may be entered now. This is initial state.
    static const char *const no_deadkey_state;
    //! This state means that deadkey was pressed but not yet released.
    //! In this state either single accented character can be entered
    //! or deadkey can be released to latch it.
    static const char *const deadkey_state;
    //! This state means that deadkey was pressed and released and thus
    //! several accented characters can be entered. Pressing deadkey
    //! again switches to initial state.
    static const char *const latched_deadkey_state;

private:
    const QScopedPointer<DeadkeyMachinePrivate> d_ptr;
};

}} // namespace Logic, MaliitKeyboard

#endif // MALIIT_KEYBOARD_DEADKEYMACHINE_H
