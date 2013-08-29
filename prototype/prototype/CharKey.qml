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

Rectangle {
    id: key
    width: panel.keyWidth
    height: panel.keyHeight
    color: "red"

    property string label: ""
    property string shifted: ""
    property string symbol1: ""
    property string symbol2: ""
    property var extended; // list of extended keys

    property string oskState: panel.activeKeypad.state

    onOskStateChanged: {
        if (panel.activeKeypad.state == "NORMAL")
            keyLabel.text = label;
        if (panel.activeKeypad.state == "SHIFTED")
            keyLabel.text = shifted;
    }

    Text {
        id: keyLabel
        text: ""
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: key
        onPressAndHold: extKeysContainer.visible = true
        onReleased: extKeysContainer.visible = false
    }

    Row {
        id: extKeysContainer
        visible: false
        Repeater {
            model: extended;
            Text {
                text: modelData
            }
        }
    }
}
