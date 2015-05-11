/*
 * Copyright 2015 Canonical Ltd.
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
import "../../keys/key_constants.js" as UI

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

            CharKey { label: "w"; shifted: "W"; extended: ["q", "1"]; extendedShifted: ["Q", "1"]; leftSide: true; }
            CharKey { label: "y"; shifted: "Y"; extended: ["ý", "2"]; extendedShifted: ["Ý", "2"] }
            CharKey { label: "e"; shifted: "E"; extended: ["é", "3", "€"]; extendedShifted: ["É", "3", "€"] }
            CharKey { label: "g"; shifted: "G"; extended: ["4"]; extendedShifted: ["4"] }
            CharKey { label: "f"; shifted: "F"; extended: ["5"]; extendedShifted: ["5"] }
            CharKey { label: "k"; shifted: "K"; extended: ["6"]; extendedShifted: ["6"] }
            CharKey { label: "s"; shifted: "S"; extended: ["7"]; extendedShifted: ["7"] }
            CharKey { label: "i"; shifted: "I"; extended: ["í", "8"]; extendedShifted: ["Í", "8"] }
            CharKey { label: "æ"; shifted: "Æ"; extended: ["9"]; extendedShifted: ["9"] }
            CharKey { label: "z"; shifted: "Z"; extended: ["c", "0"]; extendedShifted: ["C", "0"]; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CharKey { label: "x"; shifted: "X"; leftSide: true; }
            CharKey { label: "d"; shifted: "D"; extended: ["$"]; extendedShifted: ["$"] }
            CharKey { label: "l"; shifted: "L"; }
            CharKey { label: "r"; shifted: "R"; }
            CharKey { label: "t"; shifted: "T"; }
            CharKey { label: "u"; shifted: "U"; extended: ["u"]; extendedShifted: ["Ú"] }
            CharKey { label: "a"; shifted: "A"; extended: ["á", "å", "ä"]; extendedShifted: ["Á", "Å", "Ä"]}
            CharKey { label: "m"; shifted: "M"; }
            CharKey { label: "j"; shifted: "J"; extended: ["¥"]; extendedShifted: ["¥"]}
            CharKey { label: "p"; shifted: "P"; rightSide: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ShiftKey { }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "ð"; shifted: "Ð"; }
            CharKey { label: "o"; shifted: "O"; extended: ["ó"]; extendedShifted: ["Ó"]}
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "n"; shifted: "N"; }
            CharKey { label: "þ"; shifted: "Þ"; }
            CharKey { label: "ö"; shifted: "Ö"; extended: ["ø"]; extendedShifted: ["Ø"]}
            BackspaceKey { }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey; anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton; anchors.left: symShiftKey.right; height: parent.height; }
            CharKey        { id: atKey; label: "@"; shifted: "@"; extended: ["_"]; extendedShifted: ["_"]; anchors.left: languageMenuButton.right; height: parent.height; }
            SpaceKey       { id: spaceKey; anchors.left: atKey.right; anchors.right: urlKey.left; noMagnifier: true; height: parent.height; }
            UrlKey         { id: urlKey; label: ".is"; extended: [".com", ".org", ".net"]; anchors.right: dotKey.left; height: parent.height; }
            CharKey        { id: dotKey; label: "."; shifted: "."; anchors.right: enterKey.left; height: parent.height; }
            ReturnKey      { id: enterKey; anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
