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
import "keys/"
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

            HCharKey { label: "ㅂ"; shifted: "ㅃ"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            HCharKey { label: "ㅈ"; shifted: "ㅉ"; extended: ["2"]; extendedShifted: ["2"] }
            HCharKey { label: "ㄷ"; shifted: "ㄸ"; extended: ["3"]; extendedShifted: ["3"] }
            HCharKey { label: "ㄱ"; shifted: "ㄲ"; extended: ["4"]; extendedShifted: ["4"] }
            HCharKey { label: "ㅅ"; shifted: "ㅆ"; extended: ["5"]; extendedShifted: ["5"] }
            HCharKey { label: "ㅛ"; shifted: "ㅛ"; extended: ["6"]; extendedShifted: ["6"] }
            HCharKey { label: "ㅕ"; shifted: "ㅕ"; extended: ["7"]; extendedShifted: ["7"] }
            HCharKey { label: "ㅑ"; shifted: "ㅑ"; extended: ["8"]; extendedShifted: ["8"] }
            HCharKey { label: "ㅐ"; shifted: "ㅒ"; extended: ["9"]; extendedShifted: ["9"] }
            HCharKey { label: "ㅔ"; shifted: "ㅖ"; extended: ["0"]; extendedShifted: ["0"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

             HCharKey { label: "ㅁ"; shifted: "ㅁ"; leftSide: true; }
            HCharKey { label: "ㄴ"; shifted: "ㄴ"; }
            HCharKey { label: "ㅇ"; shifted: "ㅇ"; }
            HCharKey { label: "ㄹ"; shifted: "ㄹ"; }
            HCharKey { label: "ㅎ"; shifted: "ㅎ"; }
            HCharKey { label: "ㅗ"; shifted: "ㅗ"; }
            HCharKey { label: "ㅓ"; shifted: "ㅓ"; }
            HCharKey { label: "ㅏ"; shifted: "ㅏ"; }
            HCharKey { label: "ㅣ"; shifted: "ㅣ"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey {}
            HCharKey { label: "ㅋ"; shifted: "ㅋ"; }
            HCharKey { label: "ㅌ"; shifted: "ㅌ"; }
            HCharKey { label: "ㅊ"; shifted: "ㅊ"; }
            HCharKey { label: "ㅍ"; shifted: "ㅍ"; }
            HCharKey { label: "ㅠ"; shifted: "ㅠ"; }
            HCharKey { label: "ㅜ"; shifted: "ㅜ"; }
            HCharKey { label: "ㅡ"; shifted: "ㅡ"; }
            HBackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            HCharKey        { id: slashKey;    label: "/"; shifted: "/";  anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: slashKey.right; anchors.right: urlKey.left; noMagnifier: true; height: parent.height; }
            UrlKey         { id: urlKey; label: ".com"; extended: [".co.kr", ".or.kr", ".go.kr", ".ac.kr", ".kr"]; anchors.right: dotKey.left;  height: parent.height; }
            HCharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "!", "+", "%","#","/"]; extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
