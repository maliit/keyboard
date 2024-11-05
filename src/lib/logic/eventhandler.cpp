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

#include "eventhandler.h"
#include "models/layout.h"

namespace MaliitKeyboard {
namespace Logic {

//! \brief Performs event handling for Model::Layout instance, using a LayoutUpdater instance.
//!
//! Does not take ownership of either layout or updater.
EventHandler::EventHandler(QObject *parent)
    : QObject(parent)
{}


EventHandler::~EventHandler() = default;

void EventHandler::onWordCandidatePressed(QString word, bool userInput)
{
    WordCandidate candidate(userInput ? WordCandidate::SourceUser 
                                      : WordCandidate::SourcePrediction, word);
    Q_EMIT wordCandidatePressed(candidate);
}

void EventHandler::onWordCandidateReleased(QString word, bool userInput)
{
    WordCandidate candidate(userInput ? WordCandidate::SourceUser 
                                      : WordCandidate::SourcePrediction, word);
    Q_EMIT wordCandidateReleased(candidate);
}

void EventHandler::onQmlCandidateChanged(QStringList words)
{
    Q_EMIT qmlCandidateChanged(words);
}

void EventHandler::onKeyPressed(QString label, QString action)
{
    Key key;
    key.setLabel(label);

    if (action == QLatin1String("backspace"))
        key.setAction(Key::ActionBackspace);

    Q_EMIT keyPressed(key);
}

void EventHandler::onKeyReleased(QString label, QString action)
{
    Key key;
    key.setLabel(label);

    if (action == QLatin1String("return"))
        key.setAction(Key::ActionReturn);
    else if (action == QLatin1String("commit"))
        key.setAction(Key::ActionCommit);
    else if (action == QLatin1String("backspace"))
        key.setAction(Key::ActionBackspace);
    else if (action == QLatin1String("space"))
        key.setAction(Key::ActionSpace);
    else if (action == QLatin1String("shift"))
        key.setAction(Key::ActionShift);
    else if (action == QLatin1String("left"))
        key.setAction(Key::ActionLeft);
    else if (action == QLatin1String("right"))
        key.setAction(Key::ActionRight);
    else if (action == QLatin1String("up"))
        key.setAction(Key::ActionUp);
    else if (action == QLatin1String("down"))
        key.setAction(Key::ActionDown);
    else if (action == QLatin1String("home"))
        key.setAction(Key::ActionHome);
    else if (action == QLatin1String("end"))
        key.setAction(Key::ActionEnd);
    else if (action == QLatin1String("tab"))
        key.setAction(Key::ActionTab);
    else if (action == QLatin1String("ctrl"))
        key.setAction(Key::ActionControl);
    else if (action == QLatin1String("escape"))
        key.setAction(Key::ActionEscape);
    else if (action == QLatin1String("pageeup"))
        key.setAction(Key::ActionPageUp);
    else if (action == QLatin1String("pageeup"))
        key.setAction(Key::ActionPageDown);
    else if (action == "keysequence") {
        key.setCommandSequence(label);
        key.setAction(Key::ActionKeySequence);
    }
    else
        key.setAction(Key::ActionInsert);

    Q_EMIT keyReleased(key);
}

}} // namespace Logic, MaliitKeyboard
