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

            CharKey { label: "ﺽ"; extended: ["1"]; extendedShifted: ["1"] }
            CharKey { label: "ﺹ"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "ﺙ"; extended: ["3"]; extendedShifted: ["3"] }
            CharKey { label: "ﻕ"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "ﻑ"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "ﻍ"; shifted: "ﺇ"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "ﻉ"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "ﻩ"; extended: ["8"]; extendedShifted: ["8"] }
            CharKey { label: "ﺥ"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "ﺡ"; extended: ["0"]; extendedShifted: ["0"] }
            CharKey { label: "ﺝ";}
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ش"; }
            CharKey { label: "س"; }
            CharKey { label: "ي"; }
            CharKey { label: "ب"; }
            CharKey { label: "ل"; }
            CharKey { label: "ا"; shifted: "أ" }
            CharKey { label: "ت"; }
            CharKey { label: "ن"; }
            CharKey { label: "م"; }
            CharKey { label: "ك"; }
            CharKey { label: "د"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { padding: 0 }
            CharKey { label: "ئ"; }
            CharKey { label: "ء"; }
            CharKey { label: "ؤ"; }
            CharKey { label: "ر"; }
            CharKey { label: "ى"; shifted: "آ" }
            CharKey { label: "ة"; }
            CharKey { label: "و"; }
            CharKey { label: "ز"; }
            CharKey { label: "ظ"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight;

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; }
            CharKey        { id: atKey;    label: "@"; shifted: "@";     anchors.left: symShiftKey.right; }
            SpaceKey       { id: spaceKey;                               anchors.left: atKey.right; anchors.right: urlKey.left; noMagnifier: true }
            UrlKey         { id: urlKey; label: ".eg"; extended: [".iq", ".lb", ".sa", ".sy", ".jo", ".ye"]; anchors.right: dotKey.left; }
            CharKey        { id: dotKey;      label: "."; shifted: "ذ"; extended: ["?", "!"]; extendedShifted: ["?", "!"]; anchors.right: specialChar.left; }
            CharKey        { id: specialChar; label: "ط";                anchors.right: enterKey.left }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right }
        }
    } // column
}
