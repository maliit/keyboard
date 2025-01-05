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

#ifndef MALIIT_KEYBOARD_KEY_H
#define MALIIT_KEYBOARD_KEY_H

#include "models/area.h"

#include <QtCore>

namespace MaliitKeyboard {

class Key
{
public:
    enum Action {
        ActionInsert, //!< Key's label is inserted into text editor.
        ActionShift, //!< Switches keyboard to shows uppercase variant.
        ActionBackspace, //!< Key deletes previous character in text editor.
        ActionSpace, //!< Key inserts space into text editor.
        ActionCycle, //!< Key loops through set of characters when pressed in
                     //!< rapid succession and inserts currently shown key
                     //!< label into text editor after short time out.
        ActionLayoutMenu, //!< Key brings up a menu to choose other language(s).
        ActionSym, //!< Switches keyboard to symbol view.
        ActionReturn, //!< Key inserts return into text editor.
        ActionCommit, //!< Key commits preedit to text editor.
        ActionDecimalSeparator, //!< Shown key is a decimal separator that
                                //!< might require localization.
        ActionPlusMinusToggle,
        ActionSwitch, //!< Key switches through symbol view pages.
        ActionOnOffToggle,
        ActionCompose, //!< Key is used to compose character.
        ActionLeft, //!< Key moves cursor position to left, in text editor.
        ActionUp, //!< Key moves cursor position to previous line, in text editor.
        ActionRight, //!< Key moves cursor position to right, in text editor.
        ActionDown, //!< Key moves cursor position to next line, in text editor.
        ActionClose, //!< Key closes the virtual keyboard.
        ActionCommand, //!< Key executes a command.
        ActionKeySequence, //!< Key executes a key sequence.
        ActionTab, //!< Key moves cursor position by one tab, in text editor.
        ActionDead, //!< Switches keyboard to deadkey variation, using key's label as lookup.
        ActionLeftLayout, //!< Switch to left/previous language layout.
        ActionRightLayout, //!< Switch to right/next language layout.
        ActionHome, //!< Key moves cursor to beginning of text.
        ActionEnd, //!< Key moves cursor to end of text.
        NumActions
    };

    enum Style {
        StyleNormalKey,
        StyleSpecialKey,
        StyleDeadKey
    };

private:
    QPoint m_origin;
    Area m_area;
    QString m_label;
    Action m_action;
    Style m_style;
    QMargins m_margins;
    QByteArray m_icon;
    bool m_has_extended_keys: 1;
    int m_flags_padding: 7;
    QString m_command_sequence;

public:
    Key();

    // read-only properties:
    bool valid() const;
    QRect rect() const;

    QPoint origin() const;
    void setOrigin(const QPoint &origin);

    Area area() const;
    Area & rArea();
    void setArea(const Area &area);

    QString label() const;
    QString & rLabel();
    void setLabel(const QString &label);

    Action action() const;
    void setAction(Action action);

    Style style() const;
    void setStyle(Style style);

    QMargins margins() const;
    void setMargins(const QMargins &margins);

    QByteArray icon() const;
    void setIcon(const QByteArray &icon);

    bool hasExtendedKeys() const;
    void setExtendedKeysEnabled(bool enable);

    QString commandSequence() const;
    void setCommandSequence(const QString &command_sequence);
};

bool operator==(const Key &lhs,
                const Key &rhs);

bool operator!=(const Key &lhs,
                const Key &rhs);

} // namespace MaliitKeyboard

Q_DECLARE_METATYPE(MaliitKeyboard::Key)

#endif // MALIIT_KEYBOARD_KEY_H
