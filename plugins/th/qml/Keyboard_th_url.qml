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
import "keys/"

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

            ThCharKey { label: "ๅ"; shifted: "+"; extended: ["1", "๑"]; extendedShifted: ["1", "๑"]; leftSide: true; }
            ThCharKey { label: "/"; shifted: "๑"; extended: ["2", "๒"]; extendedShifted: ["2", "๒"] }
            ThCharKey { label: "-"; shifted: "๒"; extended: ["3", "๓"]; extendedShifted: ["3", "๓"] }
            ThCharKey { label: "ภ"; shifted: "๓"; extended: ["4", "๔"]; extendedShifted: ["4", "๔"] }
            ThCharKey { label: "ถ"; shifted: "๔"; extended: ["5", "๕"]; extendedShifted: ["5", "๕"] }
            ThCharKey { label: " ุ"; shifted: " ู"; }
            ThCharKey { label: " ึ"; shifted: "฿"; }
            ThCharKey { label: "ค"; shifted: "๕"; extended: ["6", "๖"]; extendedShifted: ["6", "๖"] }
            ThCharKey { label: "ต"; shifted: "๖"; extended: ["7", "๗"]; extendedShifted: ["7", "๗"] }
            ThCharKey { label: "จ"; shifted: "๗"; extended: ["8", "๘"]; extendedShifted: ["8", "๘"] }
            ThCharKey { label: "ข"; shifted: "๘"; extended: ["9", "๙"]; extendedShifted: ["9", "๙"] }
            ThCharKey { label: "ช"; shifted: "๙"; extended: ["0", "๐"]; extendedShifted: ["0", "๐"]; rightSide: true }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ThCharKey { label: "ๆ"; shifted: "๐"; leftSide: true; }
            ThCharKey { label: "ไ"; shifted: "\""; }
            ThCharKey { label: "ำ"; shifted: "ฎ"; }
            ThCharKey { label: "พ"; shifted: "ฑ"; }
            ThCharKey { label: "ะ"; shifted: "ธ"; }
            ThCharKey { label: " ั"; shifted: " ํ"; }
            ThCharKey { label: " ี"; shifted: " ๊"; }
            ThCharKey { label: "ร"; shifted: "ณ"; }
            ThCharKey { label: "น"; shifted: "ฯ"; }
            ThCharKey { label: "ย"; shifted: "ญ"; }
            ThCharKey { label: "บ"; shifted: "ฐ"; }
            ThCharKey { label: "ล"; shifted: ","; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ThCharKey { label: "ฟ"; shifted: "ฤ"; leftSide: true; }
            ThCharKey { label: "ห"; shifted: "ฆ"; }
            ThCharKey { label: "ก"; shifted: "ฏ"; }
            ThCharKey { label: "ด"; shifted: "โ"; }
            ThCharKey { label: "เ"; shifted: "ฌ"; }
            ThCharKey { label: " ้"; shifted: " ็"; }
            ThCharKey { label: " ่"; shifted: " ๋"; }
            ThCharKey { label: "า"; shifted: "ษ"; }
            ThCharKey { label: "ส"; shifted: "ศ"; }
            ThCharKey { label: "ว"; shifted: "ซ"; }
            ThCharKey { label: "ง"; shifted: "."; }
            ThCharKey { label: "ฃ"; shifted: "ฅ"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            // Shift and Backspace are as wide as other keys, because there're more keys.
            ShiftKey { padding: 0 }
            ThCharKey { label: "ผ"; shifted: "("; }
            ThCharKey { label: "ป"; shifted: ")"; }
            ThCharKey { label: "แ"; shifted: "ฉ"; }
            ThCharKey { label: "อ"; shifted: "ฮ"; }
            ThCharKey { label: " ิ"; shifted: " ฺ"; }
            ThCharKey { label: " ื"; shifted: " ์"; }
            ThCharKey { label: "ท"; shifted: "?"; }
            ThCharKey { label: "ม"; shifted: "ฒ"; }
            ThCharKey { label: "ใ"; shifted: "ฬ"; }
            ThCharKey { label: "ฝ"; shifted: "ฦ"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + Device.row_margin;

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: slashKey; label: "/"; shifted: "/";     anchors.left: languageMenuButton.right; height: parent.height; }
            UrlKey         { id: urlKey; label: ".co.th"; extended: [".com", ".th", ".co.th",".ac.th",".go.th",".mi.th",".or.th",".net.th",".in.th", ".ไทย"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "!", "+", "%","#","/"]; extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
