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
import "../keys"

KeyPad {
    anchors.fill: parent

    content: c1
    symbols: ""

    Column {
        id: c1
        anchors.fill: parent
        anchors.margins: 0;

        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "1"; noMagnifier: true; }
            CharKey { label: "2"; noMagnifier: true; }
            CharKey { label: "3"; noMagnifier: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "4"; noMagnifier: true; }
            CharKey { label: "5"; noMagnifier: true; }
            CharKey { label: "6"; noMagnifier: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "7"; noMagnifier: true; }
            CharKey { label: "8"; noMagnifier: true; }
            CharKey { label: "9"; noMagnifier: true; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.margins: 50;
            spacing: 0

            CharKey { label: "+"; annotation: "+*#"; extended: ["*", "#", "(", ")"]; noMagnifier: true; }
            CharKey { label: "0"; noMagnifier: true; }
            BackspaceKey { padding: 0; }
        }
    } // column
}

