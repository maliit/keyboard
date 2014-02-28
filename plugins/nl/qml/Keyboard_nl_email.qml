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
            CharKey { label: "e"; shifted: "E"; extended: ["ë","è","é","ê","€"]; extendedShifted: ["Ë","È","É","Ê","€"] }
            CharKey { label: "r"; shifted: "R"; }
            CharKey { label: "t"; shifted: "T"; extended: ["þ"]; extendedShifted: ["Þ"] }
            CharKey { label: "y"; shifted: "Y"; extended: ["¥"]; extendedShifted: ["¥"] }
            CharKey { label: "u"; shifted: "U"; extended: ["û","ù","ü","ú"]; extendedShifted: ["Û","Ù","Ü","Ú"] }
            CharKey { label: "i"; shifted: "I"; extended: ["ï","î","ì","í"]; extendedShifted: ["Ï","Î","Ì","Í"] }
            CharKey { label: "o"; shifted: "O"; extended: ["ô","ò","ö","ó","õ"]; extendedShifted: ["Ô","Ò","Ö","Ó","Õ"] }
            CharKey { label: "p"; shifted: "P"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "a"; shifted: "A"; extended: ["à","â","æ","ä","á","ã",]; extendedShifted: ["À","Â","Æ","Ä","Á","Ã"] }
            CharKey { label: "s"; shifted: "S"; extended: ["ß","$"]; extendedShifted: ["$"] }
            CharKey { label: "d"; shifted: "D"; }
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
            CharKey { label: "z"; shifted: "Z"; }
            CharKey { label: "x"; shifted: "X"; }
            CharKey { label: "c"; shifted: "C"; extended: ["ç"]; extendedShifted: ["Ç"] }
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "n"; shifted: "N"; extended: ["ñ"]; extendedShifted: ["Ñ"] }
            CharKey { label: "m"; shifted: "M"; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight;

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; }
            CharKey        { id: atKey;    label: "@"; shifted: "@";     anchors.left: symShiftKey.right; }
            SpaceKey       { id: spaceKey;                               anchors.left: atKey.right; anchors.right: urlKey.left; noMagnifier: true }
            UrlKey         { id: urlKey; label: ".nl"; extended: [".be", ".an", ".sx", ".sr", ".aw", ".cw"]; anchors.right: dotKey.left; }
            CharKey        { id: dotKey;      label: "."; shifted: ".";  anchors.right: enterKey.left; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}

