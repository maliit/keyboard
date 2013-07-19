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

#include "deadkeymachine.h"
#include "logic/layoutupdater.h"

namespace MaliitKeyboard {
namespace Logic {

const char *const DeadkeyMachine::no_deadkey_state = "no-deadkey";
const char *const DeadkeyMachine::latched_deadkey_state = "latched-deadkey";
const char *const DeadkeyMachine::deadkey_state = "deadkey";

class DeadkeyMachinePrivate
{
public:
    Key accent_key;

    explicit DeadkeyMachinePrivate()
        : accent_key()
    {}
};

DeadkeyMachine::DeadkeyMachine(QObject *parent)
    : QStateMachine(parent)
    , d_ptr(new DeadkeyMachinePrivate)
{}

DeadkeyMachine::~DeadkeyMachine()
{}

void DeadkeyMachine::setup(LayoutUpdater *updater)
{
    if (not updater) {
        qCritical() << __PRETTY_FUNCTION__
                    << "No updater specified. Aborting setup.";
        return;
    }

    setChildMode(QState::ExclusiveStates);

    QState *no_deadkey = 0;
    QState *deadkey = 0;
    QState *latched_deadkey = 0;

    // addState makes state machine to be a parent of passed state,
    // so we don't have to care about deleting states explicitly.
    addState(no_deadkey = new QState);
    addState(deadkey = new QState);
    addState(latched_deadkey = new QState);
    setInitialState(no_deadkey);

    no_deadkey->setObjectName(no_deadkey_state);
    deadkey->setObjectName(deadkey_state);
    latched_deadkey->setObjectName(latched_deadkey_state);

    no_deadkey->addTransition(updater, SIGNAL(deadkeyPressed()), deadkey);
    connect(no_deadkey, SIGNAL(entered()),
            updater,    SLOT(switchToMainView()));


    deadkey->addTransition(updater, SIGNAL(deadkeyCancelled()), no_deadkey);
    deadkey->addTransition(updater, SIGNAL(deadkeyReleased()), latched_deadkey);
    connect(deadkey, SIGNAL(entered()),
            updater, SLOT(switchToAccentedView()));

    latched_deadkey->addTransition(updater, SIGNAL(deadkeyCancelled()), no_deadkey);
    latched_deadkey->addTransition(updater, SIGNAL(deadkeyPressed()), no_deadkey);

    // Defer to first main loop iteration:
    QTimer::singleShot(0, this, SLOT(start()));
}

void DeadkeyMachine::setAccentKey(const Key &accent_key)
{
    Q_D(DeadkeyMachine);
    d->accent_key = accent_key;
}

Key DeadkeyMachine::accentKey() const
{
    Q_D(const DeadkeyMachine);
    return d->accent_key;
}

}} // namespace Logic, MaliitKeyboard
