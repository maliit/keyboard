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

            CharKey { label: "/"; shifted: "/"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { id: commaKey;    label: ","; shifted: ","; extended: ["2", "\"", ";", ":", "@", "&", "(", ")"]; extendedShifted: ["2", "\"", ";", ":", "@", "&", "(", ")"]; anchors.left: languageMenuButton.right; height: parent.height; }
            CharKey { id: dotKey;      label: "."; shifted: "."; extended: ["3", "?", "-", "_", "!", "+", "%","#","/"];  extendedShifted: ["3", "?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            CharKey { label: "p"; shifted: "P"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "y"; shifted: "Y"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "f"; shifted: "F"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "g"; shifted: "G"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "c"; shifted: "C"; extended: ["8", "ç"]; extendedShifted: ["8", "Ç"] }
            CharKey { label: "r"; shifted: "R"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "l"; shifted: "L"; extended: ["0"]; extendedShifted: ["0"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "a"; shifted: "A"; extended: ["ä","à","â","ª","á","å", "æ"]; extendedShifted: ["Ä","À","Â","ª","Á","Å","Æ"]; leftSide: true; }
            CharKey { label: "o"; shifted: "O"; extended: ["ö","ô","ò","ó"]; extendedShifted: ["Ö","Ô","Ò","Ó"] }
            CharKey { label: "e"; shifted: "E"; extended: ["è", "é", "ê", "ë", "€"]; extendedShifted: ["È","É", "Ê", "Ë", "€"] }
            CharKey { label: "u"; shifted: "U"; extended: ["û","ù","ú","ü"]; extendedShifted: ["Û","Ù","Ú","Ü"] }
            CharKey { label: "i"; shifted: "I"; extended: ["î","ï","ì","í"]; extendedShifted: ["Î","Ï","Ì","Í"] }
            CharKey { label: "d"; shifted: "D"; extended: ["ð"]; extendedShifted: ["Ð"] }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "t"; shifted: "T"; extended: ["þ"]; extendedShifted: ["Þ"] }
            CharKey { label: "n"; shifted: "N"; extended: ["ñ"]; extendedShifted: ["Ñ"] }
            CharKey { label: "s"; shifted: "S"; extended: ["ß","$"]; extendedShifted: ["$"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey {}
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; }
            CharKey { label: "x"; shifted: "X";  }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "m"; shifted: "M"; }
            CharKey { label: "w"; shifted: "W"; }
            CharKey { label: "v"; shifted: "V"; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: qKey; label: "q"; shifted: "Q";         anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: qKey.right; anchors.right: zKey.left; noMagnifier: true; height: parent.height; }
            CharKey        { id: zKey; label: "z"; shifted: "Z";         anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
