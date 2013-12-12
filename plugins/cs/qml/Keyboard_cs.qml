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

            CharKey { label: "q"; shifted: "Q"; }
            CharKey { label: "w"; shifted: "W"; }
            CharKey { label: "e"; shifted: "E"; extended: ["ě","é","ę","€"]; extendedShifted: ["Ě","É","Ę","€"] }
            CharKey { label: "r"; shifted: "R"; extended: ["ř","ŕ"]; extendedShifted: ["Ř","Ŕ"] }
            CharKey { label: "t"; shifted: "T"; extended: ["ť","ţ"]; extendedShifted: ["Ť","Ţ"] }
            CharKey { label: "z"; shifted: "Z"; extended: ["ž","ź","ż"]; extendedShifted: ["Ž","Ź","Ż"] }
            CharKey { label: "u"; shifted: "U"; extended: ["ů","ú","ü","ű","ù","û"]; extendedShifted: ["Û","Ú","Ü","Ű","Ù","Û"] }
            CharKey { label: "i"; shifted: "I"; extended: ["í","î"]; extendedShifted: ["Í","Î"] }
            CharKey { label: "o"; shifted: "O"; extended: ["ó","ö","ő","ô"]; extendedShifted: ["Ó","Ö","Ő","Ô"] }
            CharKey { label: "p"; shifted: "P"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "a"; shifted: "A"; extended: ["á","ä","ă","â","ą"]; extendedShifted: ["Á","Ä","Ă","Â","Ą"] }
            CharKey { label: "s"; shifted: "S"; extended: ["š","ß","ś","ş","$"]; extendedShifted: ["Š","Ś","Ş","$"] }
            CharKey { label: "d"; shifted: "D"; extended: ["ď","đ"]; extendedShifted: ["Ď","Đ"] }
            CharKey { label: "f"; shifted: "F"; }
            CharKey { label: "g"; shifted: "G"; }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; }
            CharKey { label: "l"; shifted: "L"; extended: ["ľ","ĺ","ł"]; extendedShifted: ["Ľ","Ĺ","Ł"] }
            // CharKey { label: "´"; } TODO dead keys
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey {}
            CharKey { label: "y"; shifted: "Y"; extended: ["ý", "¥"]; extendedShifted: ["Ý", "¥"] }
            CharKey { label: "x"; shifted: "X"; }
            CharKey { label: "c"; shifted: "C"; extended: ["č","ć","ç"]; extendedShifted: ["Č","Ć","Ç"] }
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "n"; shifted: "N"; extended: ["ň","ń","ñ"]; extendedShifted: ["Ň","Ń","Ñ"] }
            CharKey { label: "m"; shifted: "M"; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight;

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; }
            CharKey        { id: commaKey;    label: ","; shifted: "/";  anchors.left: languageMenuButton.right; }
            SpaceKey       { id: spaceKey;                               anchors.left: commaKey.right; anchors.right: dotKey.left; noMagnifier: true }
            CharKey        { id: dotKey;      label: "."; shifted: ".";  anchors.right: enterKey.left; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}
