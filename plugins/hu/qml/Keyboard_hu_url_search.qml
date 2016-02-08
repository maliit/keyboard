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

import QtQuick 2.4
import keys 1.0

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
            CharKey { label: "e"; shifted: "E"; extended: ["3", "é","ę","ě","€"]; extendedShifted: ["3", "É","Ę","Ě","€"] }
            CharKey { label: "r"; shifted: "R"; extended: ["4", "ŕ","ř"]; extendedShifted: ["4", "Ŕ","Ř"] }
            CharKey { label: "t"; shifted: "T"; extended: ["5", "ţ","ť"]; extendedShifted: ["5", "Ţ","Ť"] }
            CharKey { label: "z"; shifted: "Z"; extended: ["6", "ź","ż",,"ž"]; extendedShifted: ["6", "Ź","Ż","Ž"] }
            CharKey { label: "u"; shifted: "U"; extended: ["7", "ú","ü","ű","ů"]; extendedShifted: ["7", "Ú","Ü","Ű","Ů"] }
            CharKey { label: "i"; shifted: "I"; extended: ["8", "í","î"]; extendedShifted: ["8", "Í","Î"] }
            CharKey { label: "o"; shifted: "O"; extended: ["9", "ó","ö","ő","ô"]; extendedShifted: ["9", "Ó","Ö","Ő","Ô"] }
            CharKey { label: "p"; shifted: "P"; extended: ["0"]; extendedShifted: ["0"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "a"; shifted: "A"; extended: ["á","ä","â","ă","ą"]; extendedShifted: ["Á","Ä","Â","Ă","Ą"]; leftSide: true; }
            CharKey { label: "s"; shifted: "S"; extended: ["ß","ś","ş","š","$"]; extendedShifted: ["ß","Ś","Ş","Š","$"] }
            CharKey { label: "d"; shifted: "D"; extended: ["đ","ď"]; extendedShifted: ["Đ","Ď"] }
            CharKey { label: "f"; shifted: "F"; }
            CharKey { label: "g"; shifted: "G"; }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; }
            CharKey { label: "l"; shifted: "L"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey {}
            CharKey { label: "y"; shifted: "Y"; extended: ["¥"]; extendedShifted: ["¥"] }
            CharKey { label: "x"; shifted: "X"; }
            CharKey { label: "c"; shifted: "C"; extended: ["ć","č","ç"]; extendedShifted: ["Ć","Č","Ç"] }
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "n"; shifted: "N"; extended: ["ń","ň","ñ"]; extendedShifted: ["Ń","Ň","Ñ"] }
            CharKey { label: "m"; shifted: "M"; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: slashKey; label: "/"; shifted: "/";     anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: slashKey.right; anchors.right: urlKey.left; noMagnifier: true; height: parent.height; }
            UrlKey         { id: urlKey; label: ".hu"; extended: [".com"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "!", "+", "%","#","/"]; extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
