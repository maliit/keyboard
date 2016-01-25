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

            CharKey { label: "ק"; shifted: "ק"; extended: ["1"]; extendedShifted: ["1"]; leftSide: true; }
            CharKey { label: "ר"; shifted: "ר"; extended: ["2"]; extendedShifted: ["2"] }
            CharKey { label: "א"; shifted: "א"; extended: ["3"]; extendedShifted: ["3"] }
            CharKey { label: "ט"; shifted: "ט"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "ו"; shifted: "ו"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "ן"; shifted: "ן"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "ם"; shifted: "ם"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "פ"; shifted: "פ"; extended: ["8"]; extendedShifted: ["8"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ש"; shifted: "ש"; extended: ["₪"]; extendedShifted: ["₪"]; leftSide: true; }
            CharKey { label: "ד"; shifted: "ד"; }
            CharKey { label: "ג"; shifted: "ג"; }
            CharKey { label: "כ"; shifted: "כ"; }
            CharKey { label: "ע"; shifted: "ע"; }
            CharKey { label: "י"; shifted: "י"; }
            CharKey { label: "ח"; shifted: "ח"; }
            CharKey { label: "ל"; shifted: "ל"; }
            CharKey { label: "ך"; shifted: "ך"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "ף"; shifted: "ף"; extended: ["0"]; extendedShifted: ["0"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "ז"; shifted: "ז"; }
            CharKey { label: "ס"; shifted: "ס"; }
            CharKey { label: "ב"; shifted: "ב"; }
            CharKey { label: "ה"; shifted: "ה"; }
            CharKey { label: "נ"; shifted: "נ"; }
            CharKey { label: "מ"; shifted: "מ"; }
            CharKey { label: "צ"; shifted: "צ"; }
            CharKey { label: "ת"; shifted: "ת"; }
            CharKey { label: "ץ"; shifted: "ץ"; }
            BackspaceKey { padding: 0 }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: commaKey;    label: ","; shifted: ","; extended: ["'", "\"", ";", ":", "@", "&", "(", ")", "׳", "״"]; extendedShifted: ["'", ";", ":", "׳", "״"]; anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: commaKey.right; anchors.right: dotKey.left; noMagnifier: true; height: parent.height; }
            CharKey        { id: dotKey;      label: "."; shifted: "."; extended: ["?", "-", "_", "+", "!", "%", "#", "/", "־"]; extendedShifted: ["?", "-", "_", "+", "!", "%", "#", "/", "־"]; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
