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
import Ubuntu.Components 0.1

import "key_constants.js" as UI

Item {
    id: key
    width: panel.keyWidth
    height: panel.keyHeight

    property string label: ""
    property string shifted: ""
    property var extended; // list of extended keys

    property string imgNormal: UI.imageCharKey
    property string imgPressed: UI.imageCharKeyPressed

    property string oskState: panel.activeKeypad.state

    property int fontSize: units.gu( UI.fontSize );

    state: "NORMAL"

    onOskStateChanged: {
        if (panel.activeKeypad.state == "NORMAL")
            keyLabel.text = label;
        if (panel.activeKeypad.state == "SHIFTED")
            keyLabel.text = shifted;
    }

    BorderImage {
        id: buttonImage
        border { left: 22; top: 22; right: 22; bottom: 22 }
        anchors.centerIn: parent
        anchors.fill: key
        anchors.margins: units.dp( UI.keyMargins );
    }

    Text {
        id: keyLabel
        text: ""
        anchors.centerIn: parent
        font.family: UI.fontFamily
        font.pixelSize: fontSize
        color: UI.fontColor
    }

    MouseArea {
        anchors.fill: key
        onPressAndHold: extKeysContainer.visible = true
        onReleased: {
            key.state = "NORMAL"
            extKeysContainer.visible = false
        }
        onPressed: {
            key.state = "PRESSED"
        }
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

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: buttonImage
                source: imgNormal
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: buttonImage
                source: imgPressed
            }
        }
    ]
}
