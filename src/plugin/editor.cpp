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

#include "models/text.h"
#include "editor.h"

#include <QtGui/QKeyEvent>
#include <QTimer>
#include <maliit/namespace.h>

namespace MaliitKeyboard {

Editor::Editor(const EditorOptions &options,
               Model::Text *text,
               Logic::AbstractWordEngine *word_engine,
               QObject *parent)
    : AbstractTextEditor(options, text, word_engine, parent)
    , m_host(0)
{}

Editor::~Editor()
{}

void Editor::setHost(MAbstractInputMethodHost *host)
{
    m_host = host;
}

void Editor::sendPreeditString(const QString &preedit,
                               Model::Text::PreeditFace face,
                               const Replacement &replacement)
{
    if (not m_host) {
        qWarning() << __PRETTY_FUNCTION__
                   << "Host not set, ignoring.";
        return;
    }

    QList<Maliit::PreeditTextFormat> format_list;
    const int start (0);
    const int length (preedit.length());

    format_list.append(Maliit::PreeditTextFormat(start,
                                                 length,
                                                 static_cast< ::Maliit::PreeditFace>(face)));

    m_host->sendPreeditString(preedit, format_list, replacement.start,
                              replacement.length, replacement.cursor_position);
}

void Editor::sendCommitString(const QString &commit)
{
    if (not m_host) {
        qWarning() << __PRETTY_FUNCTION__
                   << "Host not set, ignoring.";
        return;
    }

    m_host->sendCommitString(commit);
}

void Editor::sendKeyEvent(const QKeyEvent &ev)
{
    if (not m_host) {
        qWarning() << __PRETTY_FUNCTION__
                     << "Host not set, ignoring.";
        return;
    }

    m_host->sendKeyEvent(ev);
}

void Editor::invokeAction(const QString &action, const QKeySequence &sequence)
{
    if (not m_host) {
        qWarning() << __PRETTY_FUNCTION__
                     << "Host not set, ignoring.";
        return;
    }

    m_host->invokeAction(action, sequence);
}

} // namespace MaliitKeyboard
