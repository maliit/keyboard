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

            CharKey { label: "q"; shifted: "Q"; }
            CharKey { label: "w"; shifted: "W"; }
            CharKey { label: "e"; shifted: "E"; extended: ["È","€"] }
            CharKey { label: "r"; shifted: "R"; }
            CharKey { label: "t"; shifted: "T"; }
            CharKey { label: "y"; shifted: "Y"; }
            CharKey { label: "u"; shifted: "U"; }
            CharKey { label: "i"; shifted: "I"; }
            CharKey { label: "o"; shifted: "O"; }
            CharKey { label: "p"; shifted: "P"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "a"; shifted: "A"; }
            CharKey { label: "s"; shifted: "S"; }
            CharKey { label: "d"; shifted: "D"; }
            CharKey { label: "f"; shifted: "F"; }
            CharKey { label: "g"; shifted: "G"; }
            CharKey { label: "h"; shifted: "H"; }
            CharKey { label: "j"; shifted: "J"; }
            CharKey { label: "k"; shifted: "K"; }
            CharKey { label: "l"; shifted: "L"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            ShiftKey { iconNormal: "images/icon_shift@18.png"; iconShifted: "images/icon_shift_upsidedown@18.png"; }
            CharKey { label: "z"; shifted: "Z"; }
            CharKey { label: "x"; shifted: "X"; }
            CharKey { label: "c"; shifted: "C"; }
            CharKey { label: "v"; shifted: "V"; }
            CharKey { label: "b"; shifted: "B"; }
            CharKey { label: "n"; shifted: "N"; }
            CharKey { label: "m"; shifted: "M"; }
            BackspaceKey {}
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            SymbolShiftKey { label: "?123"; shifted: "?123"; action: "symbols" }
            CharKey { label: ","; }
            ActionKey { action: "space" }
            CharKey { label: "."; }
            EnterKey {}
        }
    } // column
}
