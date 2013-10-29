/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import "../../keys"

KeyPad {
    anchors.fill: parent

    content: c1
    symbols: "languages/Keyboard_symbols.qml"

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "q"; }
            CharKey { label: "w"; }
            CharKey { label: "e"; }
            CharKey { label: "r"; }
            CharKey { label: "t"; }
            CharKey { label: "y"; }
            CharKey { label: "u"; }
            CharKey { label: "i"; }
            CharKey { label: "o"; }
            CharKey { label: "p"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "a"; }
            CharKey { label: "s"; }
            CharKey { label: "d"; }
            CharKey { label: "f"; }
            CharKey { label: "g"; }
            CharKey { label: "h"; }
            CharKey { label: "j"; }
            CharKey { label: "k"; }
            CharKey { label: "l"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey {}
            CharKey { label: "z"; }
            CharKey { label: "x"; }
            CharKey { label: "c"; }
            CharKey { label: "v"; }
            CharKey { label: "b"; }
            CharKey { label: "n"; }
            CharKey { label: "m"; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight;

            SymbolShiftKey { id: symShiftKey; label: "符号";              anchors.left: parent.left; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; }
            CharKey        { id: commaKey;    label: ","; shifted: "，";  anchors.left: languageMenuButton.right; }
            SpaceKey       { id: spaceKey;                               anchors.left: commaKey.right; anchors.right: dotKey.left; noMagnifier: true }
            CharKey        { id: dotKey;      label: "。"; shifted: "。"; anchors.right: enterKey.left; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}
