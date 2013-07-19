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

#include "tagbinding.h"

namespace MaliitKeyboard {

TagBinding::TagBinding(Action action,
                       const QString &label,
                       const QString &secondary_label,
                       const QString &accents,
                       const QString &accented_labels,
                       const QString &cycle_set,
                       const QString &sequence,
                       const QString &icon,
                       bool dead,
                       bool quick_pick,
                       bool rtl,
                       bool enlarge)
    : m_action(action)
    , m_label(label)
    , m_secondary_label(secondary_label)
    , m_accents(accents)
    , m_accented_labels(accented_labels)
    , m_cycle_set(cycle_set)
    , m_sequence(sequence)
    , m_icon(icon)
    , m_dead(dead)
    , m_quick_pick(quick_pick)
    , m_rtl (rtl)
    , m_enlarge (enlarge)
{}

TagBinding::Action TagBinding::action() const
{
    return m_action;
}

const QString TagBinding::label() const
{
    return m_label;
}

const QString TagBinding::secondary_label() const
{
    return m_secondary_label;
}

const QString TagBinding::accents() const
{
    return m_accents;
}

const QString TagBinding::accented_labels() const
{
    return m_accented_labels;
}

const QString TagBinding::cycle_set() const
{
    return m_cycle_set;
}

const QString TagBinding::sequence() const
{
    return m_sequence;
}

bool TagBinding::dead() const
{
    return m_dead;
}

bool TagBinding::quick_pick() const
{
    return m_quick_pick;
}

bool TagBinding::rtl() const
{
    return m_rtl;
}

bool TagBinding::enlarge() const
{
    return m_enlarge;
}

const QString TagBinding::icon() const
{
    return m_icon;
}

const TagModifiersPtrs TagBinding::modifiers() const
{
    return m_modifiers;
}

void TagBinding::appendModifiers(const TagModifiersPtr &modifiers)
{
    m_modifiers.append(modifiers);
}

} // namespace MaliitKeyboard
