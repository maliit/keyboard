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

#ifndef MALIIT_KEYBOARD_EVENTHANDLER_H
#define MALIIT_KEYBOARD_EVENTHANDLER_H

#include "models/wordcandidate.h"

namespace MaliitKeyboard {

class Key;

namespace Logic {


class EventHandler
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(EventHandler)

public:
    explicit EventHandler(QObject *parent = nullptr);
    ~EventHandler() override;

    Q_INVOKABLE void onWordCandidatePressed(QString word, bool userInput);
    Q_INVOKABLE void onWordCandidateReleased(QString word, bool userInput);
    Q_INVOKABLE void onKeyPressed(QString label, QString action = QString());
    Q_INVOKABLE void onKeyReleased(QString label, QString action = QString());
    Q_INVOKABLE void onQmlCandidateChanged(QStringList words);

    // Key signals:
    Q_SIGNAL void keyPressed(const Key &key);
    Q_SIGNAL void keyReleased(const Key &key);
    Q_SIGNAL void wordCandidatePressed(const WordCandidate &candidate);
    Q_SIGNAL void wordCandidateReleased(const WordCandidate &candidate);

    Q_SIGNAL void languageChangeRequested(QString languageId);
    Q_SIGNAL void qmlCandidateChanged(QStringList words);
};

}} // namespace Logic, MaliitKeyboard

#endif // MALIIT_KEYBOARD_EVENTHANDLER_H
