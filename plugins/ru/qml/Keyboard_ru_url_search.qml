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

            CharKey { label: "й"; shifted: "Й"; extended: ["1"]; extendedShifted: ["1"] }
            CharKey { label: "ц"; shifted: "Ц"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "у"; shifted: "Y"; extended: ["3", "ў"]; extendedShifted: ["3", "Ў"] }
            CharKey { label: "к"; shifted: "К"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "e"; shifted: "E"; extended: ["5", "ë", "€"]; extendedShifted: ["5", "Ë", "€"] }
            CharKey { label: "н"; shifted: "Н"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "г"; shifted: "Г"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "ш"; shifted: "Ш"; extended: ["8"]; extendedShifted: ["8"] }
            CharKey { label: "щ"; shifted: "Щ"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "з"; shifted: "З"; extended: ["0"]; extendedShifted: ["0"] }
            CharKey { label: "х"; shifted: "Х"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ф"; shifted: "Ф"; }
            CharKey { label: "ы"; shifted: "Ы"; }
            CharKey { label: "в"; shifted: "В"; }
            CharKey { label: "a"; shifted: "A"; }
            CharKey { label: "п"; shifted: "П"; }
            CharKey { label: "р"; shifted: "Р"; }
            CharKey { label: "о"; shifted: "О"; }
            CharKey { label: "л"; shifted: "Л"; }
            CharKey { label: "д"; shifted: "Д"; }
            CharKey { label: "ж"; shifted: "Ж"; }
            CharKey { label: "э"; shifted: "Э"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { padding: 0 }
            CharKey { label: "я"; shifted: "Я"; }
            CharKey { label: "ч"; shifted: "Ч"; }
            CharKey { label: "с"; shifted: "С"; }
            CharKey { label: "м"; shifted: "М"; }
            CharKey { label: "і"; shifted: "І"; }
            CharKey { label: "и"; shifted: "И"; }
            CharKey { label: "т"; shifted: "Т"; }
            CharKey { label: "ь"; shifted: "Ь"; }
            CharKey { label: "б"; shifted: "Б"; }
            CharKey { label: "ю"; shifted: "Ю"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight;
            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; }
            CharKey        { id: slashKey; label: "/"; shifted: "/";     anchors.left: languageMenuButton.right; }
            SpaceKey       { id: spaceKey;                               anchors.left: slashKey.right; anchors.right: urlKey.left; noMagnifier: true }
            UrlKey         { id: urlKey; label: ".ru"; extended: [".ua",".su",".kg",".рф","укр",".by",".tj"]; anchors.right: dotKey.left; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "!"]; extendedShifted: ["?", "!"]; anchors.right: extraCharKey.left; }
            CharKey        { id: extraCharKey; label: "ъ"; shifted: "Ъ"; anchors.right: enterKey.left; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}
