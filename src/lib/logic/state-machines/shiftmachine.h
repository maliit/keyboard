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

#ifndef MALIIT_KEYBOARD_SHIFTMACHINE_H
#define MALIIT_KEYBOARD_SHIFTMACHINE_H

#include "abstractstatemachine.h"
#include <QtCore>

namespace MaliitKeyboard {
namespace Logic {

class LayoutUpdater;

class ShiftMachine
    : public QStateMachine
    , public AbstractStateMachine
{
    Q_OBJECT
    Q_DISABLE_COPY(ShiftMachine)

public:
    explicit ShiftMachine(QObject *parent = 0);
    virtual ~ShiftMachine();

    virtual void setup(LayoutUpdater *updater);

    //! This state means that neither shift nor caps-lock wasn't pressed.
    //! Entered characters are lowercased. This is initial state.
    static const char *const no_shift_state;
    //! This state means that shift was pressed but not yet released.
    //! Now user can either release shift state to latch it or press
    //! a key to enter one uppercased character.
    static const char *const shift_state;
    //! This state means that shift was pressed and released and thus
    //! user can enter several uppercased characters.
    static const char *const latched_shift_state;
    //! Same as latched shift?
    static const char *const caps_lock_state;
};

}} // namespace Logic, MaliitKeyboard

#endif // MALIIT_KEYBOARD_SHIFTMACHINE_H
