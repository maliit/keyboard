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

            CharKey { label: "й"; shifted: "Й"; }
            CharKey { label: "ц"; shifted: "Ц"; }
            CharKey { label: "у"; shifted: "Y"; extended: ["ў"]; extendedShifted: ["Ў"] }
            CharKey { label: "к"; shifted: "К"; }
            CharKey { label: "e"; shifted: "E"; extended: ["ë", "€"]; extendedShifted: ["Ë", "€"] }
            CharKey { label: "н"; shifted: "Н"; }
            CharKey { label: "г"; shifted: "Г"; }
            CharKey { label: "ш"; shifted: "Ш"; }
            CharKey { label: "щ"; shifted: "Щ"; }
            CharKey { label: "з"; shifted: "З"; }
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
            CharKey        { id: slashKey; label: "/"; shifted: "/";     anchors.left: symShiftKey.right; }
            UrlKey         { id: urlKey; label: ".ru"; extended: [".ua",".su",".kg",".рф","укр",".by",".tj"]; anchors.right: dotKey.left; }
            CharKey        { id: dotKey;      label: "."; shifted: ".";  anchors.right: extraCharKey.left; }
            CharKey        { id: extraCharKey; label: "ъ"; shifted: "Ъ"; anchors.right: enterKey.left; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}
