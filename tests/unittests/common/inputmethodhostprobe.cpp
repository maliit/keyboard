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

#include "inputmethodhostprobe.h"
#include <QtDebug>

InputMethodHostProbe::InputMethodHostProbe()
    : m_commit_string_history()
    , m_last_preedit_string()
    , m_last_key_event(QEvent::None, 0, Qt::NoModifier)
    , m_key_event_count(0)
    , m_last_preedit_text_format_list()
    , m_last_replace_start(0)
    , m_last_replace_length(0)
    , m_last_cursor_pos(0)
    , m_preedit_string_sent(false)
{}

QString InputMethodHostProbe::commitStringHistory() const
{
    return m_commit_string_history;
}

void InputMethodHostProbe::sendCommitString(const QString &string,
                                            int replace_start,
                                            int replace_length,
                                            int cursor_pos)
{
    Q_UNUSED(replace_start)
    Q_UNUSED(replace_length)
    Q_UNUSED(cursor_pos)

    m_commit_string_history.append(string);
}

QString InputMethodHostProbe::lastPreeditString() const
{
    return m_last_preedit_string;
}

int InputMethodHostProbe::lastReplaceStart() const
{
    return m_last_replace_start;
}

int InputMethodHostProbe::lastReplaceLength() const
{
    return m_last_replace_length;
}

int InputMethodHostProbe::lastCursorPos() const
{
    return m_last_cursor_pos;
}

bool InputMethodHostProbe::preeditStringSent() const
{
    return m_preedit_string_sent;
}

void InputMethodHostProbe::sendPreeditString(const QString &string,
                                             const QList<Maliit::PreeditTextFormat> &format,
                                             int replace_start,
                                             int replace_length,
                                             int cursor_pos)
{
    m_preedit_string_sent = true;
    m_last_preedit_string = string;
    m_last_preedit_text_format_list = format;
    m_last_replace_start = replace_start;
    m_last_replace_length = replace_length;
    m_last_cursor_pos = cursor_pos;
}

const KeyEvent & InputMethodHostProbe::lastKeyEvent() const
{
    return m_last_key_event;
}

int InputMethodHostProbe::keyEventCount() const
{
    return m_key_event_count;
}

void InputMethodHostProbe::sendKeyEvent(const QKeyEvent& event, Maliit::EventRequestType)
{
    m_last_key_event.m_type = event.type();
    m_last_key_event.m_key = event.key();
    m_last_key_event.m_modifiers = event.modifiers();
    ++m_key_event_count;
    Q_EMIT keyEventSent(m_last_key_event.m_type, m_last_key_event.m_key, m_last_key_event.m_modifiers);
}

QList<Maliit::PreeditTextFormat> InputMethodHostProbe::lastPreeditTextFormatList() const
{
    return m_last_preedit_text_format_list;
}
