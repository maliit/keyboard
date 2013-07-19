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

#include "viewmachine.h"
#include "logic/layoutupdater.h"

namespace MaliitKeyboard {
namespace Logic {

const char *const ViewMachine::main_state = "main";
const char *const ViewMachine::symbols0_state = "symbols0";
const char *const ViewMachine::symbols1_state = "symbols1";

ViewMachine::ViewMachine(QObject *parent)
    : QStateMachine(parent)
{}

ViewMachine::~ViewMachine()
{}

void ViewMachine::setup(LayoutUpdater *updater)
{
    if (not updater) {
        qCritical() << __PRETTY_FUNCTION__
                    << "No updater specified. Aborting setup.";
        return;
    }

    setChildMode(QState::ExclusiveStates);

    QState *main = 0;
    QState *symbols0 = 0;
    QState *symbols1 = 0;

    // addState makes state machine to be a parent of passed state,
    // so we don't have to care about deleting states explicitly.
    addState(main = new QState);
    addState(symbols0 = new QState);
    addState(symbols1 = new QState);
    setInitialState(main);

    main->setObjectName(main_state);
    symbols0->setObjectName(symbols0_state);
    symbols1->setObjectName(symbols1_state);

    main->addTransition(updater, SIGNAL(symKeyReleased()), symbols0);
    connect(main,    SIGNAL(entered()),
            updater, SLOT(switchToMainView()));

    symbols0->addTransition(updater, SIGNAL(symKeyReleased()), main);
    symbols0->addTransition(updater, SIGNAL(symSwitcherReleased()), symbols1);
    connect(symbols0, SIGNAL(entered()),
            updater,  SLOT(switchToPrimarySymView()));

    symbols1->addTransition(updater, SIGNAL(symKeyReleased()), main);
    symbols1->addTransition(updater, SIGNAL(symSwitcherReleased()), symbols0);
    connect(symbols1, SIGNAL(entered()),
            updater,  SLOT(switchToSecondarySymView()));

    // Defer to first main loop iteration:
    QTimer::singleShot(0, this, SLOT(start()));
}

}} // namespace Logic, MaliitKeyboard
