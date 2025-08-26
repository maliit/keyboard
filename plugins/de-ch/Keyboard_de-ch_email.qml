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
import MaliitKeyboard 2.0
import keys 1.0

KeyPad {
    // This is the crucial property to link the visual layout to the
    // system's German-Swiss (de-ch) XKB layout.
    xkbLayoutName: "ch(de)"

    anchors.fill: parent
    content: c1
    symbols: "languages/Keyboard_symbols.qml"

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        // Row 1: The QWERTZ row
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            CharKey { label: "q"; shifted: "Q"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { label: "w"; shifted: "W"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "e"; shifted: "E"; extended: ["3", "è", "é", "ë", "ê", "€"]; extendedShifted: ["3", "È","É", "Ë", "Ê", "€"] }
            CharKey { label: "r"; shifted: "R"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "t"; shifted: "T"; extended: ["5"]; extendedShifted: ["5"] }
            // Z and Y are swapped in the QWERTZ layout
            CharKey { label: "z"; shifted: "Z"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "u"; shifted: "U"; extended: ["7", "ù","û","ū","ü","ú"]; extendedShifted: ["7", "Ù","Û","Ū","Ü","Ú"] }
            CharKey { label: "i"; shifted: "I"; extended: ["8", "î","ï","ì","í","į","ī"]; extendedShifted: ["8", "Î","Ï","Ì","Í","Į","Ī"] }
            CharKey { label: "o"; shifted: "O"; extended: ["9", "ô","œ","ö","ò","ó","õ","ō","ø"]; extendedShifted: ["9", "Ô","Œ","Ö","Ò","Ó","Õ","Ō","Ø"] }
            CharKey { label: "p"; shifted: "P"; extended: ["0"]; extendedShifted: ["0"]; rightSide: true; }
        }

        // Row 2: The ASDF row
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            CharKey { label: "a"; shifted: "A"; extended: ["ä","à","á","â"]; extendedShifted: ["Ä","À","Á","Â"]; leftSide: true; }
            CharKey { label: "s"; shifted: "S"; }
            CharKey { label: "d"; shifted: "D"; }
            CharKey { label: "f"; shifted: "F"; }
            CharKey { label: "g"; shifted: "G"; }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; }
            CharKey { label: "l"; shifted: "L"; }
            // Corrected placement for "ö" and "ä"
            CharKey { label: "ö"; shifted: "Ö"; extended: ["õ","ø"]; extendedShifted: ["Õ","Ø"]; }
            CharKey { label: "ä"; shifted: "Ä"; extended: ["à","á","â"]; extendedShifted: ["À","Á","Â"]; rightSide: true; }
        }

        // Row 3: The YXZCVB row (Z and Y are swapped)
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            ShiftKey {}
            // Corrected placement for "y"
            CharKey { label: "y"; shifted: "Y"; extended: ["ÿ","¥"]; extendedShifted: ["Ÿ","¥"] }
            CharKey { label: "x"; shifted: "X"; }
            CharKey { label: "c"; shifted: "C"; extended: ["ç","č","ć"]; extendedShifted: ["Ç","Č","Ć"]; }
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "n"; shifted: "N"; }
            CharKey { label: "m"; shifted: "M"; }
            // Corrected placement for "ü"
            CharKey { label: "ü"; shifted: "Ü"; }
            BackspaceKey {}
        }

        // Row 4: The modifier/function row (standard layout)
        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            height: panel.keyHeight + Device.row_margin

            SymbolShiftKey { id: symShiftKey; anchors.left: parent.left; height: parent.height; }
            LanguageKey { id: languageMenuButton; anchors.left: symShiftKey.right; height: parent.height; }
            CharKey {
                id: commaKey;
                label: ","; shifted: ",";
                extended: ["'", "\"", ";", ":", "@", "&", "(", ")"];
                extendedShifted: ["'", "\"", ";", ":", "@", "&", "(", ")"];
                anchors.left: languageMenuButton.right; height: parent.height;
            }
            SpaceKey {
                id: spaceKey;
                anchors.left: commaKey.right; anchors.right: dotKey.left;
                noMagnifier: true; height: parent.height;
            }
            CharKey {
                id: dotKey;
                label: "."; shifted: ".";
                extended: ["?", "-", "_", "!", "+", "%","#","/"];
                extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"];
                anchors.right: enterKey.left; height: parent.height;
            }
            ReturnKey { id: enterKey; anchors.right: parent.right; height: parent.height; }
        }
    }
}

