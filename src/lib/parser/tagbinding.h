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

#ifndef MALIIT_KEYBOARD_TAG_BINDING_H
#define MALIIT_KEYBOARD_TAG_BINDING_H

#include <QtGlobal>
#include <QString>

#include "alltagtypes.h"

namespace MaliitKeyboard {

class TagBinding
{
    Q_DISABLE_COPY(TagBinding)

public:
    enum Action {
        Insert,
        Shift,
        Backspace,
        Space,
        Cycle,
        LayoutMenu,
        Sym,
        Return,
        Commit,
        DecimalSeparator,
        PlusMinusToggle,
        Switch,
        OnOffToggle,
        Compose,
        Left,
        Up,
        Right,
        Down,
        Close,
        Command
    };

    TagBinding(Action action,
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
               bool enlarge);

    Action action() const;
    const QString label() const;
    const QString secondary_label() const;
    const QString accents() const;
    const QString accented_labels() const;
    const QString cycle_set() const;
    const QString sequence() const;
    const QString icon() const;
    bool dead() const;
    bool quick_pick() const;
    bool rtl() const;
    bool enlarge() const;
    const TagModifiersPtrs modifiers() const;

    void appendModifiers(const TagModifiersPtr &modifiers);

private:
    const Action m_action;
    const QString m_label;
    const QString m_secondary_label;
    const QString m_accents;
    const QString m_accented_labels;
    const QString m_cycle_set;
    const QString m_sequence;
    const QString m_icon;
    const bool m_dead;
    const bool m_quick_pick;
    const bool m_rtl;
    const bool m_enlarge;
    TagModifiersPtrs m_modifiers;
};

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_TAG_BINDING_H
