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

            CharKey { label: "ч"; shifted: "Ч"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { label: "ш"; shifted: "Ш"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "е"; shifted: "Е"; extended: ["3"]; extendedShifted: ["3"] }
            CharKey { label: "р"; shifted: "Р"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "т"; shifted: "Т"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "ъ"; shifted: "Ъ"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "у"; shifted: "У"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "и"; shifted: "И"; extended: ["8"]; extendedShifted: ["8"] }
            CharKey { label: "о"; shifted: "О"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "п"; shifted: "П"; extended: ["0"]; extendedShifted: ["0"] }
            CharKey { label: "я"; shifted: "Я"; }
            CharKey { label: "щ"; shifted: "Щ"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "а"; shifted: "А"; leftSide: true; }
            CharKey { label: "с"; shifted: "С"; }
            CharKey { label: "д"; shifted: "Д"; }
            CharKey { label: "ф"; shifted: "Ф"; }
            CharKey { label: "г"; shifted: "Г"; }
            CharKey { label: "х"; shifted: "Х"; }
            CharKey { label: "й"; shifted: "Й"; }
            CharKey { label: "к"; shifted: "К"; }
            CharKey { label: "л"; shifted: "Л"; }
            CharKey { label: "ь"; shifted: "ь"; extended: ["ѝ"]; extendedShifted: ["Ѝ"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey {}
            CharKey { label: "з"; shifted: "З"; }
            CharKey { label: "ж"; shifted: "Ж"; }
            CharKey { label: "ц"; shifted: "Ц"; }
            CharKey { label: "в"; shifted: "В"; }
            CharKey { label: "б"; shifted: "Б"; }
            CharKey { label: "н"; shifted: "Н"; }
            CharKey { label: "м"; shifted: "М"; }
            CharKey { label: "ю"; shifted: "Ю"; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: slashKey; label: "/"; shifted: "/";     anchors.left: languageMenuButton.right; height: parent.height; }
            UrlKey         { id: urlKey; label: ".бг"; extended: [".bg", ".com", ".net", ".org"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "!", "+", "%","#","/"]; extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
