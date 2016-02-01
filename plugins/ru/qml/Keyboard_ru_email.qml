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

            CharKey { label: "й"; shifted: "Й"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { label: "ц"; shifted: "Ц"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "у"; shifted: "У"; extended: ["3", "ў"]; extendedShifted: ["3", "Ў"] }
            CharKey { label: "к"; shifted: "К"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "е"; shifted: "Е"; extended: ["5", "ё", "€"]; extendedShifted: ["5", "Ё", "€"] }
            CharKey { label: "н"; shifted: "Н"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "г"; shifted: "Г"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "ш"; shifted: "Ш"; extended: ["8"]; extendedShifted: ["8"] }
            CharKey { label: "щ"; shifted: "Щ"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "з"; shifted: "З"; extended: ["0"]; extendedShifted: ["0"] }
            CharKey { label: "х"; shifted: "Х"; }
            CharKey { label: "ъ"; shifted: "Ъ"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ф"; shifted: "Ф"; leftSide: true; }
            CharKey { label: "ы"; shifted: "Ы"; }
            CharKey { label: "в"; shifted: "В"; }
            CharKey { label: "а"; shifted: "А"; }
            CharKey { label: "п"; shifted: "П"; }
            CharKey { label: "р"; shifted: "Р"; }
            CharKey { label: "о"; shifted: "О"; }
            CharKey { label: "л"; shifted: "Л"; }
            CharKey { label: "д"; shifted: "Д"; }
            CharKey { label: "ж"; shifted: "Ж"; }
            CharKey { label: "э"; shifted: "Э"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { padding: 0; width: symShiftKey.width; }
            CharKey { label: "я"; shifted: "Я"; }
            CharKey { label: "ч"; shifted: "Ч"; }
            CharKey { label: "с"; shifted: "С"; }
            CharKey { label: "м"; shifted: "М"; }
            CharKey { label: "и"; shifted: "И"; extended: ["і", "І"]; }
            CharKey { label: "т"; shifted: "Т"; }
            CharKey { label: "ь"; shifted: "Ь"; }
            CharKey { label: "б"; shifted: "Б"; }
            CharKey { label: "ю"; shifted: "Ю"; }
            BackspaceKey { padding: 0; width: enterKey.width; }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);
            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: atKey;    label: "@"; shifted: "@";     anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: atKey.right; anchors.right: urlKey.left; noMagnifier: true; height: parent.height; }
            UrlKey         { id: urlKey; label: ".ru"; extended: [".com", ".ua",".su",".kg",".рф",".укр",".by",".tj"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: ".";  extended: ["?", "-", "_", "!", "+", "%","#","/"]; extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
