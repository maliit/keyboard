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

            CharKey { label: "ض"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { label: "ص"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "ق"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "ف"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "غ"; shifted: "إ"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "ع"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "ه"; extended: ["8"]; extendedShifted: ["8"] }
            CharKey { label: "خ"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "ح"; extended: ["0"]; extendedShifted: ["0"] }
            CharKey { label: "ج"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ش"; leftSide: true; }
            CharKey { label: "س"; }
            CharKey { label: "ی"; }
            CharKey { label: "ب"; }
            CharKey { label: "ل"; }
            CharKey { label: "ا"; shifted: "آ" }
            CharKey { label: "ت"; shifted: "ث" }
            CharKey { label: "ن"; }
            CharKey { label: "م"; }
            CharKey { label: "چ"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ظ"; }
            CharKey { label: "ط"; }
            CharKey { label: "ز"; }
            CharKey { label: "ر"; }
            CharKey { label: "د"; shifted: "ذ" }
            CharKey { label: "پ"; }
            CharKey { label: "و"; }
            CharKey { label: "ک"; }
            CharKey { label: "گ"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: atKey;    label: "@"; shifted: "@";     anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: atKey.right; anchors.right: urlKey.left; noMagnifier: true; height: parent.height; }
            UrlKey         { id: urlKey; label: ".com"; extended: [".co.uk", ".net", ".org", ".edu", ".gov", ".ac.uk"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "!", "+", "%","#","/"]; extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
