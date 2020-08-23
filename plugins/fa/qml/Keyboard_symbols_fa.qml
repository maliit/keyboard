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
    anchors.fill: parent

    content: c1

    Column {
        id: c1
        anchors.fill: parent
        anchors.margins: 0;

        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "۱"; extended: ["1"]; shifted: "~"; leftSide: true; }
            CharKey { label: "۲"; extended: ["2"]; shifted: "`"; }
            CharKey { label: "۳"; extended: ["3"]; shifted: "|"; }
            CharKey { label: "۴"; extended: ["4"]; shifted: "•"; }
            CharKey { label: "۵"; extended: ["5"]; shifted: "√"; }
            CharKey { label: "۶"; extended: ["6"]; shifted: "π"; }
            CharKey { label: "۷"; extended: ["7"]; shifted: "÷"; }
            CharKey { label: "۸"; extended: ["8"]; shifted: "×"; }
            CharKey { label: "۹"; extended: ["9"]; shifted: "¶"; }
            CharKey { label: "۰"; extended: ["0","∅","ⁿ"]; shifted: "∆"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "٫"; shifted: "@"; leftSide: true; }
            CharKey { label: "٬"; shifted: "#"; }
            CharKey { label: "﷼"; shifted: ">‏"; extended: ["$","€","¢","₱","£","¥"]; }
            CharKey { label: "٪"; shifted: "<‏"; extended: ["%", "‰"]; }
            CharKey { label: "*"; extended: ["٭", "★"]; shifted: "^"; }
            CharKey { label: "-"; shifted: "°"; extended: ["_", "ـ", "—", "–", "·"]; }
            CharKey { label: "+"; shifted: "="; extended: ["±"]; }
            CharKey { label: ")‏"; shifted: "}‏"; extended: ["}‏", "﴾‏", ">‏", "]‏"]; switchBackFromSymbols: true; }
            CharKey { label: "(‏"; shifted: "{‏"; extended: ["{‏", "﴿‏", "<‏", "[‏"]; switchBackFromSymbols: true; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            OneTwoKey { label: "=\\×"; shifted: "۱۲۳؟"; }
            CharKey { label: "»‏"; shifted: "©"; switchBackFromSymbols: true; }
            CharKey { label: "«‏"; shifted: "®"; switchBackFromSymbols: true; }
            CharKey { label: "/"; shifted: "\\"; }
            CharKey { label: ":"; shifted: "™"; }
            CharKey { label: "؛"; shifted: "℅"; }
            CharKey { label: "!"; shifted: "]‏"; extended: ["¡"]; }
            CharKey { label: "؟"; shifted: "[‏"; extended: ["?", "¿"]; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + Device.bottom_margin * 2;

            SymbolShiftKey { id: symShiftKey; label: "اب‌پ"; shifted: "اب‌پ"; anchors.left: parent.left; height: parent.height; }
            CharKey        { id: commaKey;    label: "،"; shifted: "،";     anchors.left: symShiftKey.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                                  anchors.left: commaKey.right; anchors.right: zwj.left; noMagnifier: true; height: parent.height }
            CharKey        { id: zwj; label: "🢒⸽🢐"; shifted: "🢒⸽🢐"; valueToSubmit: "‍"; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: ".";     anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                                  anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
