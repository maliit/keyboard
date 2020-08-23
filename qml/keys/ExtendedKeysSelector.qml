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

import MaliitKeyboard 2.0

KeyPopover {
    id: popover
    enabled: false

    property variant extendedKeysModel
    property alias keys: rowOfKeys.children
    property alias rowX: rowOfKeys.x
    property alias rowY: rowOfKeys.y
    property int fontSize: 0

    property int __width: 0
    property string __commitStr: ""

    onExtendedKeysModelChanged: {
        if (extendedKeysModel && extendedKeysModel.length > 1) {
            // Place the first key in the middle of the model so that it gets
            // selected by default
            var middleKey = Math.floor(extendedKeysModel.length / 2);
            var reorderedModel = extendedKeysModel.slice(0); // Ensure the array is cloned
            reorderedModel.splice(extendedKeysModel.length % 2 == 0 ? middleKey : middleKey + 1, 0, extendedKeysModel[0]);
            reorderedModel.shift();
            keyRepeater.model = reorderedModel;
        } else {
            keyRepeater.model = extendedKeysModel;
        }

        var longestKey = 1;
        // Calculate font size based on longest key
        if (extendedKeysModel != null) {
            for(var i = 0; i < extendedKeysModel.length; i++) {
                if (extendedKeysModel[i].length > longestKey) {
                    longestKey = extendedKeysModel[i].length;
                }
            }
            fontSize = (fullScreenItem.landscape ? (panel.keyHeight / 2) : (panel.keyHeight / 2.8))
                       * (4 / (longestKey >= 2 ? (longestKey <= 6 ? longestKey + 2.5 : 8) : 4));
        }
    }

    onEnabledChanged: {
        canvas.extendedKeysShown = enabled
    }

    Rectangle {
        id: popoverBackground

        anchors.centerIn: anchorItem
        anchors.verticalCenterOffset: -Device.popoverTopMargin

        width: {
            if (rowOfKeys.width < keypad.keyWidth)
                return keypad.keyWidth;
            else
                return rowOfKeys.width;
        }

        height: rowOfKeys.height

        color: Theme.charKeyColor
        radius: Device.gu(0.8)
        border {
            width: Device.gu(0.1)
        	color: Theme.popupBorderColor
        }

        onXChanged: {

            if (x < Device.popoverEdgeMargin) {
                anchorItem.x += Math.abs(x) + Device.popoverEdgeMargin;
                return
            }

            var rightEdge = (x + width)
            if ( rightEdge > (panel.width - Device.popoverEdgeMargin)) {
                var diff = rightEdge - panel.width
                anchorItem.x -= diff + Device.popoverEdgeMargin;
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: closePopover();
    }

    Row {
        id: rowOfKeys
        anchors.centerIn: anchorItem
        anchors.verticalCenterOffset: -Device.popoverTopMargin

        Component.onCompleted: __width = 0

        Repeater {
            id: keyRepeater
            model: extendedKeysModel

            Item {
                id: key
                width: textCell.width + Device.popoverCellPadding;

                height: panel.keyHeight;

                property alias commitStr: textCell.text
                property bool highlight: false
                opacity: highlight ? 1.0 : 0.6

                Text {
                    id: textCell
                    anchors.centerIn: parent;
                    text: modelData
                    font.family: Theme.fontFamily
                    font.pixelSize: fontSize
                    font.weight: Font.Light
                    color: key.highlight ? Theme.selectionColor : Theme.fontColor
                    Component.onCompleted: __width += (textCell.width + Device.popoverCellPadding);
                }

                function commit(skipAutoCaps) {
                    key.highlight = false;
                    event_handler.onKeyPressed(modelData);
                    event_handler.onKeyReleased(modelData);
                    if (panel.autoCapsTriggered) {
                        panel.autoCapsTriggered = false;
                    } else if (!skipAutoCaps) {
                        if (popover.parent.activeKeypadState === "SHIFTED" && popover.parent.state === "CHARACTERS")
                            popover.parent.activeKeypadState = "NORMAL"
                    }
                    popover.closePopover();
                }
            }
        }
    }

    function enableMouseArea()
    {
        extendedKeysMouseArea.enabled = true
    }

    function __restoreAssignedKey()
    {
        currentlyAssignedKey.state = "NORMAL"
    }

    function closePopover()
    {
        extendedKeysModel = null;
        // Forces re-evaluation of anchor position, in case we change
        // orientation and then open the popover for the same key again
        currentlyAssignedKey = null;
        popover.enabled = false
    }
}


