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

            CharKey { label: "a"; shifted: "A"; extended: ["1", "à", "á", "â", "ä", "å", "ã", "ā", "æ"]; extendedShifted: ["1", "À", "Á", "Â", "Ä", "Å", "Ã", "Ā", "Æ"]; leftSide: true }
            CharKey { label: "z"; shifted: "Z"; extended: ["2", "ž", "ź", "ż"]; extendedShifted: ["2", "Ž", "Ź", "Ż"] }
            CharKey { label: "e"; shifted: "E"; extended: ["3", "è", "é", "ê", "ë", "ē", "ę", "ɛ", "€"]; extendedShifted: ["3", "È", "É", "Ê", "Ë", "Ē", "Ę", "Ɛ", "€"] }
            CharKey { label: "r"; shifted: "R"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "t"; shifted: "T"; extended: ["5", "ṫ"]; extendedShifted: ["5", "Ṫ"] }
            CharKey { label: "y"; shifted: "Y"; extended: ["6", "ŷ", "ÿ", "¥"]; extendedShifted: ["6", "Ŷ", "Ÿ", "¥"] }
            CharKey { label: "u"; shifted: "U"; extended: ["7", "ù", "ú", "ü", "û", "ū"]; extendedShifted: ["7", "Ù", "Ú", "Ü", "Û", "Ū"] }
            CharKey { label: "i"; shifted: "I"; extended: ["8", "ì", "í", "ï", "î", "ī", "į"]; extendedShifted: ["8", "Ì", "Í", "Ï", "Î", "Ī", "Į"] }
            CharKey { label: "o"; shifted: "O"; extended: ["9", "ò", "ó", "ö", "ô", "ō", "œ", "ø", "ɵ"]; extendedShifted: ["9", "Ò", "Ó", "Ö", "Ô", "Ō", "Œ", "Ø", "Ɵ"] }
            CharKey { label: "p"; shifted: "P"; extended: ["ṗ"]; extendedShifted: ["Ṗ"]; rightSide: true }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0
            CharKey { label: "q"; shifted: "Q"; leftSide: true }
            CharKey { label: "s"; shifted: "S"; extended: ["ṡ", "ß", "ś", "š", "$"]; extendedShifted: ["Ṡ", "Ś", "Š", "$"] }
            CharKey { label: "d"; shifted: "D"; extended: ["ḋ"]; extendedShifted: ["Ḋ"] }
            CharKey { label: "f"; shifted: "F"; extended: ["ḟ"]; extendedShifted: ["Ḟ"] }
            CharKey { label: "g"; shifted: "G"; extended: ["ġ"]; extendedShifted: ["Ġ"] }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; }
            CharKey { label: "l"; shifted: "L"; extended: ["£", "ł"]; extendedShifted: ["£", "Ł"] }
            CharKey { label: "m"; shifted: "M"; extended: ["ṁ"]; extendedShifted: ["Ṁ"]; rightSide: true }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { width: panel.keyWidth * 2 }

            CharKey { label: "w"; shifted: "W"; extended: ["ŵ"]; extendedShifted: ["Ŵ"] }
            CharKey { label: "x"; shifted: "X"; }
            CharKey { label: "c"; shifted: "C"; extended: ["ċ", "ç", "ć", "č"]; extendedShifted: ["Ċ", "Ç", "Ć", "Č"] }
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "b"; shifted: "B"; extended: ["ḃ"]; extendedShifted: ["Ḃ"] }
            CharKey { label: "n"; shifted: "N"; extended: ["ñ", "ń"]; extendedShifted: ["Ñ", "Ń"] }
            BackspaceKey { width: panel.keyWidth * 2 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.bottom_margin*2);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: commaKey;    label: ","; shifted: "/"; extended: ["'", ";", ":"]; anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: commaKey.right; anchors.right: dotKey.left; noMagnifier: true; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "!"]; extendedShifted: ["?", "!"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
