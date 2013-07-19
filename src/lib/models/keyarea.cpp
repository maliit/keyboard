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

#include "keyarea.h"

namespace MaliitKeyboard {

KeyArea::KeyArea()
    : m_keys()
    , m_origin()
    , m_area()
{}

bool KeyArea::hasKeys() const
{
    return (not m_keys.isEmpty());
}

QRect KeyArea::rect() const
{
    return QRect(m_origin, m_area.size());
}

QPoint KeyArea::origin() const
{
    return m_origin;
}

void KeyArea::setOrigin(const QPoint &origin)
{
    m_origin = origin;
}

QVector<Key> KeyArea::keys() const
{
    return m_keys;
}

QVector<Key> & KeyArea::rKeys()
{
    return m_keys;
}

void KeyArea::setKeys(const QVector<Key> &keys)
{
    m_keys = keys;
}

Area KeyArea::area() const
{
    return m_area;
}

Area & KeyArea::rArea()
{
    return m_area;
}

void KeyArea::setArea(const Area &area)
{
    m_area = area;
}

bool operator==(const KeyArea &lhs,
                const KeyArea &rhs)
{
    return (lhs.area() == rhs.area()
            && lhs.keys() == rhs.keys());
}

bool operator!=(const KeyArea &lhs,
                const KeyArea &rhs)
{
    return (not (lhs == rhs));
}

} // namespace MaliitKeyboard
