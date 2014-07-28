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

            CharKey { label: "љ"; shifted: "Љ"; }
            CharKey { label: "њ"; shifted: "Њ"; }
            CharKey { label: "е"; shifted: "Е"; extended: ["€"]; extendedShifted: ["€"] }
            CharKey { label: "р"; shifted: "Р"; }
            CharKey { label: "т"; shifted: "Т"; }
            CharKey { label: "з"; shifted: "З"; }
            CharKey { label: "у"; shifted: "У"; }
            CharKey { label: "и"; shifted: "И"; }
            CharKey { label: "о"; shifted: "О"; }
            CharKey { label: "п"; shifted: "П"; }
            CharKey { label: "ш"; shifted: "Ш"; }
            CharKey { label: "ђ"; shifted: "Ђ"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "а"; shifted: "А"; }
            CharKey { label: "с"; shifted: "С"; }
            CharKey { label: "д"; shifted: "Д"; }
            CharKey { label: "ф"; shifted: "Ф"; }
            CharKey { label: "г"; shifted: "Г"; }
            CharKey { label: "х"; shifted: "Х"; }
            CharKey { label: "ј"; shifted: "Ј"; }
            CharKey { label: "к"; shifted: "К"; }
            CharKey { label: "л"; shifted: "Л"; }
            CharKey { label: "ч"; shifted: "Ч"; }
            CharKey { label: "ћ"; shifted: "Ћ"; }
            CharKey { label: "ж"; shifted: "Ж"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { padding: 0 }
            CharKey { label: "џ"; shifted: "Џ"; }
            CharKey { label: "ц"; shifted: "Ц"; }
            CharKey { label: "в"; shifted: "В"; }
            CharKey { label: "б"; shifted: "Б"; }
            CharKey { label: "н"; shifted: "Н"; }
            CharKey { label: "м"; shifted: "М"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight;
            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; }
            CharKey        { id: slashKey;    label: "/"; shifted: "/";     anchors.left: symShiftKey.right; }
            UrlKey         { id: urlKey; label: ".rs"; extended: [".com",".net",".org",".срб"]; anchors.right: dotKey.left; }
            CharKey        { id: dotKey;      label: "."; shifted: ".";  anchors.right: enterKey.left; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}
