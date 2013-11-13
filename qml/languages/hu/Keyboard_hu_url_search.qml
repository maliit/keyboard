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
            CharKey { label: "e"; shifted: "E"; extended: ["é","ę","ě","€"]; extendedShifted: ["É","Ę","Ě","€"] }
            CharKey { label: "r"; shifted: "R"; extended: ["ŕ","ř"]; extendedShifted: ["Ŕ","Ř"] }
            CharKey { label: "t"; shifted: "T"; extended: ["ţ","ť"]; extendedShifted: ["Ţ","Ť"] }
            CharKey { label: "z"; shifted: "Z"; extended: ["ź","ż",,"ž"]; extendedShifted: ["Ź","Ż","Ž"] }
            CharKey { label: "u"; shifted: "U"; extended: ["ú","ü","ű","ů"]; extendedShifted: ["Ú","Ü","Ű","Ů"] }
            CharKey { label: "i"; shifted: "I"; extended: ["í","î"]; extendedShifted: ["Í","Î"] }
            CharKey { label: "o"; shifted: "O"; extended: ["ó","ö","ő","ô"]; extendedShifted: ["Ó","Ö","Ő","Ô"] }
            CharKey { label: "p"; shifted: "P"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "a"; shifted: "A"; extended: ["á","ä","â","ă","ą"]; extendedShifted: ["Á","Ä","Â","Ă","Ą"] }
            CharKey { label: "s"; shifted: "S"; extended: ["ß","ś","ş","š","$"]; extendedShifted: ["ß","Ś","Ş","Š","$"] }
            CharKey { label: "d"; shifted: "D"; extended: ["đ","ď"]; extendedShifted: ["Đ","Ď"] }
            CharKey { label: "f"; shifted: "F"; }
            CharKey { label: "g"; shifted: "G"; }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; }
            CharKey { label: "l"; shifted: "L"; }
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

            height: panel.keyHeight;

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; }
            CharKey        { id: slashKey;    label: "/"; shifted: "/";  anchors.left: symShiftKey.right; }
            SpaceKey       { id: spaceKey;                               anchors.left: slashKey.right; anchors.right: urlKey.left; noMagnifier: true }
            UrlKey         { id: urlKey; label: ".hu";                   anchors.right: dotKey.left; }
            CharKey        { id: dotKey;      label: "."; shifted: ".";  anchors.right: enterKey.left; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}
