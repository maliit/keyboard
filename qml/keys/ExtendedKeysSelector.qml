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
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import MaliitKeyboard 2.0

KeyPopover {
    id: popover
    enabled: false

    property variant extendedKeysModel
    property alias keys: rowOfKeys.children
    property alias rowX: rowOfKeys.x
    property alias rowY: rowOfKeys.y
    property int fontSize: 0

    // A readonly variable to check if our grid has multiple rows
    readonly property bool multirow: rowOfKeys.columns < (extendedKeysModel ? extendedKeysModel.length : 0)

    property string __commitStr: ""

    onExtendedKeysModelChanged: {
        if (extendedKeysModel && extendedKeysModel.length > 1) {
            // Reset columns to length, to avoid having weird positioning
            // when switching extended keys
            rowOfKeys.columns = extendedKeysModel.length;

            // Place the first key in the middle of the model so that it gets
            // selected by default
            var middleKey = Math.floor(extendedKeysModel.length / 2);
            if (extendedKeysModel.length > 5) {
                middleKey -= 1;
            }
            var reorderedModel = extendedKeysModel.slice(0); // Ensure the array is cloned
            var defaultKey = extendedKeysModel[0];
            reorderedModel.splice(extendedKeysModel.length % 2 == 0 ? middleKey : middleKey + 1, 0, extendedKeysModel[0]);
            reorderedModel.shift();
            if (reorderedModel.length > 5) {
                rowOfKeys.columns = Math.ceil(reorderedModel.length / 2);
                reorderedModel.reverse();
            }
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

        // Invisible tooltip to copy the qqc2 style colors from
        ToolTip {
            id: tip
            visible: false
        }

        color: tip.background.color
        radius: Device.gu(0.8)
        border {
            width: tip.background.border.width
            color: tip.background.border.color
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

    GridLayout {
        id: rowOfKeys
        anchors.centerIn: anchorItem
        anchors.verticalCenterOffset: -Device.popoverTopMargin

        Repeater {
            id: keyRepeater
            model: extendedKeysModel

            Item {
                id: key
                width: panel.keyWidth

                height: panel.keyHeight;

                property alias commitStr: textCell.text
                property bool highlight: false
                opacity: highlight ? 1.0 : 0.6

                // Invisible text field to get selection colors from
                TextField {
                    id: textArea
                    width: 0
                    height: 0
                    visible: false
                }

                Label {
                    id: textCell
                    anchors.centerIn: parent;
                    text: modelData
                    font.pixelSize: fontSize
                    font.weight: Font.Light
                    color: key.highlight ? textArea.selectionColor : textArea.color
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
                        if (popover.parent.activeKeypadState === "CONTROL" && popover.parent.state === "CHARACTERS")
                            popover.parent.activeKeypadState = "NORMAL"
                    }
                    popover.closePopover();
                }
            }
        }
    }

    // Determine which extended key we're underneath when swiping,
    // highlight it and set it as the currentExtendedKey (to be committed
    // when press is released)
    function evaluateSelectorSwipe(mouseX, mouseY) {
        let currentExtendedKey = null;
        if (enabled && currentlyAssignedKey != null) {
            var keyMapping = mapToItem(currentlyAssignedKey, rowX, rowY);
            var mx = mouseX - keyMapping.x;
            var my = mouseY - keyMapping.y;
            for (var i = 0; i < keys.length; i++) {
                var posX = keys[i].x;
                var posY = keys[i].y;
                if (mx > posX && mx < (posX + keys[i].width)
                    && my > posY && my < (posY + (keys[i].height * (posY == rowOfKeys.height - panel.keyHeight ? 2 : 1)))) {
                    if (!keys[i].highlight) {
                        Feedback.startPressEffect();
                    }
                    keys[i].highlight = true;
                    currentExtendedKey = keys[i];
                } else if('highlight' in keys[i]) {
                    keys[i].highlight = false;
                }
            }
        }
        return currentExtendedKey;
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
