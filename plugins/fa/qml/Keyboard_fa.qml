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
    symbols: "languages/Keyboard_symbols.qml"

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ض"; extended: ["۱","1"]; shifted: "ض"; extendedShifted: ["۱","1"]; leftSide: true; }
            CharKey { label: "ص"; extended: ["۲","2","٬"]; shifted: "ص"; extendedShifted: ["۲","2","٬"]; }
            CharKey { label: "ث"; extended: ["۳","3","٫"]; shifted: "ث"; extendedShifted: ["۳","3","٫"]; }
            CharKey { label: "ق"; extended: ["۴","4"]; shifted: "ق"; extendedShifted: ["۴","4"]; }
            CharKey { label: "ف"; extended: ["۵","5"]; shifted: "ف"; extendedShifted: ["۵","5"]; }
            CharKey { label: "غ"; extended: ["۶","6"]; shifted: "غ"; extendedShifted: ["۶","6"]; }
            CharKey { label: "ع"; extended: ["۷","7"]; shifted: "ع"; extendedShifted: ["۷","7"]; }
            CharKey { label: "ه"; extended: ["۸","8"]; shifted: "ه"; extendedShifted: ["۸","8"]; }
            CharKey { label: "خ"; extended: ["۹","9"]; shifted: "خ"; extendedShifted: ["۹","9"]; }
            CharKey { label: "ح"; extended: ["۰","0"]; shifted: "ح"; extendedShifted: ["۰","0"];  }
            CharKey { label: "ج"; shifted: "ج"; rightSide: true; }
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
            CharKey { label: "ت"; extended: ["ة"]; shifted: "ت"; extendedShifted: ["ة"]; }
            CharKey { label: "ن"; shifted: "ن"; }
            CharKey { label: "م"; shifted: "م"; }
            CharKey { label: "ک"; shifted: "ک"; }
            CharKey { label: "گ"; shifted: "گ";  rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0
            CharKey { label: "ظ"; extended: ["ك"]; shifted: "ط"; extendedShifted: ["ك"]; }
            CharKey { label: "ط"; shifted: "ط"; }
            CharKey { label: "ژ"; shifted: "ژ"; }
            CharKey { label: "ز"; shifted: "ز"; }
            CharKey { label: "ر"; shifted: "ر"; }
            CharKey { label: "ذ"; shifted: "ذ"; }
            CharKey { label: "د"; shifted: "د"; }
            CharKey { label: "پ"; shifted: "پ"; }
            CharKey { label: "و"; extended: ["ؤ"]; shifted: "و"; extendedShifted: ["ؤ"]; }
            CharKey { label: "چ"; shifted: "چ"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + Device.row_margin;

            SymbolShiftKey { id: symShiftKey; label: "۱۲۳؟"; shifted: "۱۲۳؟";  anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: commaKey;    label: "،"; extended: ["«‏", "ْ", "ّ", ")‏", "(‏", "»‏", "ُ", "ِ", "َ", "ٌ", "ٍ", "ً"]; shifted: "،"; extendedShifted: ["«‏", "ْ", "ّ", ")‏", "(‏", "»‏", "ُ", "ِ", "َ", "ٌ", "ٍ", "ً"]; anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: commaKey.right; anchors.right: zwnj.left; noMagnifier: true; height: parent.height; }
            CharKey        { id: zwnj; label: "🢐⸽🢒"; shifted: "🢐⸽🢒"; valueToSubmit: "‌"; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; extended: ["؟", "٪", ":", "؛", "!","/","ـ"];  shifted: "."; extendedShifted: ["؟", "٪", ":", "؛", "!","/","ـ"];  anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
