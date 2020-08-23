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
    symbols: "lib/fa/Keyboard_symbols_fa.qml"

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ض"; extended: ["۱","1"]; shifted: "ض"; extendedShifted: ["۱","1"]; leftSide: true; }
            CharKey { label: "ص"; extended: ["۲","2","٬"]; shifted: "ص"; extendedShifted: ["۲","2","٬"]; }
            CharKey { label: "ق"; extended: ["۳","3","٫"]; shifted: "ق"; extendedShifted: ["۳","3","٫"]; }
            CharKey { label: "ف"; extended: ["۴","4"]; shifted: "ف"; extendedShifted: ["۴","4"]; }
            CharKey { label: "غ"; extended: ["۵","5"]; shifted: "غ"; extendedShifted: ["۵","5"]; }
            CharKey { label: "ع"; extended: ["۶","6"]; shifted: "ع"; extendedShifted: ["۶","6"]; }
            CharKey { label: "ه"; extended: ["۷","7","ه‍","ة"]; shifted: "ه"; extendedShifted: ["۷","7","ه‍","ة"]; }
            CharKey { label: "خ"; extended: ["۸","8"]; shifted: "خ"; extendedShifted: ["۸","8"]; }
            CharKey { label: "ح"; extended: ["۹","9"]; shifted: "ح"; extendedShifted: ["۹","9"]; }
            CharKey { label: "ج"; extended: ["۰","0"]; shifted: "ج"; extendedShifted: ["۰","0"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ش"; shifted: "ش"; leftSide: true; }
            CharKey { label: "س"; shifted: "س"; }
            CharKey { label: "ی"; extended: ["ٔ","ئ","ء","ي"]; shifted: "ی"; extendedShifted: ["ٔ","ئ","ء","ي"]; }
            CharKey { label: "ب"; shifted: "ب"; }
            CharKey { label: "ل"; shifted: "ل"; }
            CharKey { label: "ا"; extended: ["آ","أ","إ","ء"]; shifted: "ا"; extendedShifted: ["آ","أ","إ","ء"]; }
            CharKey { label: "ت"; extended: ["ث","ة"]; shifted: "ت"; extendedShifted: ["ث","ة"]; }
            CharKey { label: "ن"; shifted: "ن"; }
            CharKey { label: "م"; shifted: "م"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ط"; extended: ["ظ","ك"]; shifted: "ط"; extendedShifted: ["ظ","ك"]; }
            CharKey { label: "ز"; extended: ["ژ"]; shifted: "ز"; extendedShifted: ["ژ"]; }
            CharKey { label: "ر"; shifted: "ر"; }
            CharKey { label: "د"; extended: ["ذ"]; shifted: "د"; extendedShifted: ["ذ"]; }
            CharKey { label: "پ"; shifted: "پ"; }
            CharKey { label: "و"; extended: ["ؤ"]; shifted: "و"; extendedShifted: ["ؤ"]; }
            CharKey { label: "ک"; shifted: "ک"; }
            CharKey { label: "گ"; shifted: "گ"; }
            CharKey { label: "چ"; shifted: "چ"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + Device.row_margin;

            SymbolShiftKey { id: symShiftKey; label: "۱۲۳؟"; shifted: "۱۲۳؟";  anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: slashKey; label: "/"; shifted: "/";  anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: slashKey.right; anchors.right: zwnj.left; noMagnifier: true; height: parent.height; }
            CharKey        { id: zwnj; label: "🢐⸽🢒"; shifted: "🢐⸽🢒"; valueToSubmit: "‌"; anchors.right: urlKey.left; height: parent.height; }
            UrlKey         { id: urlKey; label: ".com"; extended: [".ir", ".net", ".org", ".edu", ".gov.ir", ".ac.ir"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; extended: ["؟", "٪", ":", "؛", "!","/","ـ"];  shifted: "."; extendedShifted: ["؟", "٪", ":", "؛", "!","/","ـ"];  anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
