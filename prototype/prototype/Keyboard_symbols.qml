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

KeyPad {
    anchors.fill: parent

    content: c1

    Column {
        id: c1
        anchors.fill: parent
        anchors.margins: 0;

        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "1"; shifted: "€"; }
            CharKey { label: "2"; shifted: "£"; }
            CharKey { label: "3"; shifted: "$"; extended: ["È","€"] }
            CharKey { label: "4"; shifted: "¥"; }
            CharKey { label: "5"; shifted: "₹"; }
            CharKey { label: "6"; shifted: "%"; }
            CharKey { label: "7"; shifted: "<"; }
            CharKey { label: "8"; shifted: ">"; }
            CharKey { label: "9"; shifted: "["; }
            CharKey { label: "0"; shifted: "]"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "*"; shifted: "`"; }
            CharKey { label: "#"; shifted: "^"; }
            CharKey { label: "+"; shifted: "|"; }
            CharKey { label: "-"; shifted: "_"; }
            CharKey { label: "="; shifted: "§"; }
            CharKey { label: "("; shifted: "{"; }
            CharKey { label: ")"; shifted: "}"; }
            CharKey { label: "!"; shifted: "¡"; }
            CharKey { label: "?"; shifted: "¿"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            ShiftKey { label: "1/2"; shifted: "2/2"; fontSize: units.gu(3); }
            CharKey { label: "@"; shifted: "«"; }
            CharKey { label: "~"; shifted: "»"; }
            CharKey { label: "/"; shifted: "¨"; }
            CharKey { label: "\\"; shifted: "“"; }
            CharKey { label: "'"; shifted: "”"; }
            CharKey { label: ";"; shifted: "„"; }
            CharKey { label: ":"; shifted: "&"; }
            BackspaceKey {}
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.horizontalCenter: parent.horizontalCenter;

            height: panel.keyHeight;

            SymbolShiftKey { id: symShiftKey; label: "ABC"; shifted: "ABC"; action: "symbols"; anchors.left: parent.left; }
            CharKey { id: commaKey; label: ",";  anchors.left: symShiftKey.right; }
            ActionKey { anchors.left: commaKey.right; anchors.right: dotKey.left; action: "space"; }
            CharKey { id: dotKey; label: "."; anchors.right: enterKey.left; }
            EnterKey { id: enterKey; anchors.right: parent.right }
        }
    } // column
}
