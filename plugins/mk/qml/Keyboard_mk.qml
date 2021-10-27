/*
 * Copyright 2013 Canonical Ltd.
 * Copyright 2021 Boris Mihailov
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

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "љ"; shifted: "Љ"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { label: "њ"; shifted: "Њ"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "е"; shifted: "Е"; extended: ["3", "е́", "е̑", "ѐ", "е̏", "е̄", "€"]; extendedShifted: ["3", "Е́", "Е̑", "Ѐ", "Е̏", "Е̄", "€"] }
            CharKey { label: "р"; shifted: "Р"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "т"; shifted: "Т"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "ѕ"; shifted: "Ѕ"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "у"; shifted: "У"; extended: ["7", "у́", "у̑", "у̀", "у̏", "ӯ"]; extendedShifted: ["7", "У́", "У̑", "У̀", "У̏", "Ӯ"] }
            CharKey { label: "и"; shifted: "И"; extended: ["8", "и́", "и̑", "ѝ", "и̏", "ӣ"]; extendedShifted: ["8", "И́", "И̑", "Ѝ", "И̏", "Ӣ"] }
            CharKey { label: "о"; shifted: "О"; extended: ["9", "о́", "о̑", "о̀", "о̏", "о̄"]; extendedShifted: ["9", "О́", "О̑", "О̀", "О̏", "О̄"] }
            CharKey { label: "п"; shifted: "П"; extended: ["0"]; extendedShifted: ["0"] }
            CharKey { label: "ш"; shifted: "Ш"; rightSide: true; } 
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "а"; shifted: "А"; extended: ["а́", "а̑", "а̀", "а̏", "а̄"]; extendedShifted: ["А́", "А̑", "А̀", "А̏", "А̄"]; leftSide: true; }
            CharKey { label: "с"; shifted: "С"; }
            CharKey { label: "д"; shifted: "Д"; }
            CharKey { label: "ф"; shifted: "Ф"; }
            CharKey { label: "г"; shifted: "Г"; }
            CharKey { label: "х"; shifted: "Х"; }
            CharKey { label: "ј"; shifted: "Ј"; }
            CharKey { label: "к"; shifted: "К"; }
            CharKey { label: "л"; shifted: "Л"; }
            CharKey { label: "ч"; shifted: "Ч"; }
            CharKey { label: "ќ"; shifted: "Ќ"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { width: panel.keyWidth }
            CharKey { label: "з"; shifted: "З"; }
            CharKey { label: "џ"; shifted: "Џ"; }
            CharKey { label: "ц"; shifted: "Ц"; }
            CharKey { label: "в"; shifted: "В"; }
            CharKey { label: "б"; shifted: "Б"; }
            CharKey { label: "н"; shifted: "Н"; }
            CharKey { label: "м"; shifted: "М"; }
            CharKey { label: "ѓ"; shifted: "Ѓ"; }
            CharKey { label: "ж"; shifted: "Ж"; }
            BackspaceKey { width: panel.keyWidth }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + Device.row_margin;

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: commaKey;    label: ","; shifted: ","; extended: ["'", "\"", ";", ":", "@", "&", "(", ")"]; extendedShifted: ["'", "\"", ";", ":", "@", "&", "(", ")"]; anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: commaKey.right; anchors.right: dotKey.left; noMagnifier: true; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "!", "+", "%","#","/"];  extendedShifted: ["?", "-", "_", "!", "+", "%","#","/"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
