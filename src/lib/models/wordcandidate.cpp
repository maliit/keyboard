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

#include "wordcandidate.h"

namespace MaliitKeyboard {

WordCandidate::WordCandidate()
    : m_origin()
    , m_area()
    , m_label()
    , m_source(SourceUnknown)
    , m_word()
{}

WordCandidate::WordCandidate(Source source, const QString &word)
    : m_origin()
    , m_area()
    , m_label()
    , m_source(source)
    , m_word(word)
{
    if (source == WordCandidate::SourceUser) {
        m_label.setText(QString(QT_TR_NOOP("Add '%1' to user dictionary")).arg(word));
    } else {
        m_label.setText(word);
    }
}

bool WordCandidate::valid() const
{
    return (m_area.size().isValid()
            && not m_label.text().isEmpty());
}

QRect WordCandidate::rect() const
{
    return QRect(m_origin, m_area.size());
}

QPoint WordCandidate::origin() const
{
    return m_origin;
}

void WordCandidate::setOrigin(const QPoint &origin)
{
    m_origin = origin;
}

Area WordCandidate::area() const
{
    return m_area;
}

Area & WordCandidate::rArea()
{
    return m_area;
}

void WordCandidate::setArea(const Area &area)
{
    m_area = area;
}

Label WordCandidate::label() const
{
    return m_label;
}

Label & WordCandidate::rLabel()
{
    return m_label;
}

void WordCandidate::setLabel(const Label &label)
{
    m_label = label;
}

WordCandidate::Source WordCandidate::source() const
{
    return m_source;
}

void WordCandidate::setSource(Source source)
{
    m_source = source;
}

QString WordCandidate::word() const
{
    return m_word;
}

void WordCandidate::setWord(const QString &word)
{
    m_word = word;
}

bool operator==(const WordCandidate &lhs,
                const WordCandidate &rhs)
{
    return (lhs.origin() == rhs.origin()
            && lhs.area() == rhs.area()
            && lhs.label() == rhs.label()
            && lhs.source() == rhs.source());
}

bool operator!=(const WordCandidate &lhs,
                const WordCandidate &rhs)
{
    return (not (lhs == rhs));
}

} // namespace MaliitKeyboard
