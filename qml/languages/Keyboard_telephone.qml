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
import "../keys"

KeyPad {
    anchors.fill: parent

    content: c1
    symbols: ""

    Column {
        id: c1
        anchors.fill: parent
        anchors.leftMargin: units.gu(1);
        anchors.rightMargin: units.gu(1);
        anchors.bottomMargin: units.gu(1);

        property double numKeyWidth: panel.keyWidth - units.gu(0.75);
        property double numKeyHeight: panel.keyHeight - units.gu(1);

        spacing: units.gu(1);

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: units.gu(0.5);

            CharKey { label: "1"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "2"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "3"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "#"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: units.gu(0.5);

            CharKey { label: "4"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "5"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "6"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "*"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: units.gu(0.5);

            CharKey { label: "7"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "8"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "9"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            BackspaceKey { padding: 0; width: c1.numKeyWidth; height: c1.numKeyHeight; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: units.gu(0.5);

            CharKey { label: "+"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: "0"; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            CharKey { label: ""; enabled: false; noMagnifier: true; width: c1.numKeyWidth; height: c1.numKeyHeight; }
            ReturnKey { padding: 0; width: c1.numKeyWidth; height: c1.numKeyHeight; }
        }
    } // column
}

