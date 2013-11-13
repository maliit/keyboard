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

#include "key.h"

namespace MaliitKeyboard {

Key::Key()
    : m_origin()
    , m_area()
    , m_label()
    , m_action(ActionInsert)
    , m_style(StyleNormalKey)
    , m_margins()
    , m_icon()
    , m_has_extended_keys(false)
{}

bool Key::valid() const
{
    return (m_area.size().isValid()
            && (not m_label.isEmpty() || m_action != Key::ActionCommit));
}

QRect Key::rect() const
{
    return QRect(m_origin, m_area.size());
}

QPoint Key::origin() const
{
    return m_origin;
}

void Key::setOrigin(const QPoint &origin)
{
    m_origin = origin;
}

Area Key::area() const
{
    return m_area;
}

Area & Key::rArea()
{
    return m_area;
}

void Key::setArea(const Area &area)
{
    m_area = area;
}

QString Key::label() const
{
    return m_label;
}

QString & Key::rLabel()
{
    return m_label;
}

void Key::setLabel(const QString &label)
{
    m_label = label;
}

Key::Action Key::action() const
{
    return m_action;
}

void Key::setAction(Action action)
{
    m_action = action;
}

Key::Style Key::style() const
{
    return m_style;
}

void Key::setStyle(Style style)
{
    m_style = style;
}

QMargins Key::margins() const
{
    return m_margins;
}

void Key::setMargins(const QMargins &margins)
{
    m_margins = margins;
}

QByteArray Key::icon() const
{
    return m_icon;
}

void Key::setIcon(const QByteArray &icon)
{
    m_icon = icon;
}

bool Key::hasExtendedKeys() const
{
    return m_has_extended_keys;
}

void Key::setExtendedKeysEnabled(bool enable)
{
    m_has_extended_keys = enable;
}

QString Key::commandSequence() const
{
    return m_command_sequence;
}

void Key::setCommandSequence(const QString &command_sequence)
{
    m_command_sequence = command_sequence;
}

bool operator==(const Key &lhs,
                const Key &rhs)
{
    return (lhs.origin() == rhs.origin()
            && lhs.area() == rhs.area()
            && lhs.label() == rhs.label()
            && lhs.icon() == rhs.icon());
}

bool operator!=(const Key &lhs,
                const Key &rhs)
{
    return (not (lhs == rhs));
}

} // namespace MaliitKeyboard
