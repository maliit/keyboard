/*
 * Copyright 2016 Canonical Ltd.
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

import QtQuick 2.4
import "../../keys"
import "../../keys/key_constants.js" as UI

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

            CharKey { label: "q"; shifted: "Q"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { label: "w"; shifted: "W"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "e"; shifted: "E"; extended: ["3", "ē", "€"]; extendedShifted: ["3", "Ē", "€"] }
            CharKey { label: "r"; shifted: "R"; extended: ["4","ŗ"]; extendedShifted: ["4", "Ŗ"] }
            CharKey { label: "t"; shifted: "T"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "y"; shifted: "Y"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "u"; shifted: "U"; extended: ["7", "ū"]; extendedShifted: ["7", "Ū"] }
            CharKey { label: "i"; shifted: "I"; extended: ["8", "ī"]; extendedShifted: ["8", "Ī"] }
            CharKey { label: "o"; shifted: "O"; extended: ["9", "ō"]; extendedShifted: ["9", "Ō"] }
            CharKey { label: "p"; shifted: "P"; extended: ["0"]; extendedShifted: ["0"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "a"; shifted: "A"; extended: ["ā"]; extendedShifted: ["Ā"]; leftSide: true; }
            CharKey { label: "s"; shifted: "S"; extended: ["š","$"]; extendedShifted: ["Š","$"] }
            CharKey { label: "d"; shifted: "D"; }
            CharKey { label: "f"; shifted: "F"; }
            CharKey { label: "g"; shifted: "G"; extended: ["ģ"]; extendedShifted: ["Ģ"]; }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; extended: ["ķ"]; extendedShifted: ["Ķ"]; }
            CharKey { label: "l"; shifted: "L"; extended: ["ļ"]; extendedShifted: ["Ļ"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { padding: 0 }
            CharKey { label: "z"; shifted: "Z"; extended: ["ž"]; extendedShifted: ["Ž"] }
            CharKey { label: "x"; shifted: "X"; }
            CharKey { label: "c"; shifted: "C"; extended: ["č", "¢"]; extendedShifted: ["Č", "¢"] }
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "n"; shifted: "N"; extended: ["ņ"]; extendedShifted: ["Ņ"] }
            CharKey { label: "m"; shifted: "M"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: slashKey; label: "/"; shifted: "/";     anchors.left: languageMenuButton.right; height: parent.height; }
            UrlKey         { id: urlKey; label: ".lv"; extended: [".gov.lv", ".com", ".net", ".org", ".edu", ".gov"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "!", "+", "%","#","/"]; extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
