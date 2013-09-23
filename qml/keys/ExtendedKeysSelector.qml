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
import Ubuntu.Components.ListItems 0.1 as ListItem
import Ubuntu.Components.Popups 0.1

import QtQuick.Window 2.0

import "key_constants.js" as UI

Item {
    id: popover
    visible: false

    property variant extendedKeysModel
    property Item currentlyAssignedKey

    property int __width: 0
    property string __commitStr: ""

    onCurrentlyAssignedKeyChanged:
    {
        if (currentlyAssignedKey == null)
            return;

        __repositionPopoverTo(currentlyAssignedKey);
    }

    ///
    // Item gets repositioned above the currently active key on keyboard.
    // extended keys area will center on top of this

    Item {
        id: anchorItem
        width: panel.keyWidth
        height: panel.keyHeight
    }

    Rectangle {
        id: popoverBackground

        anchors.centerIn: anchorItem
        width: {
            if (rowOfKeys.width < keypad.keyWidth)
                return keypad.keyWidth;
            else
                return rowOfKeys.width;
        }

        height: rowOfKeys.height
        radius: 17
        color: "white"
        border.color: "lightGray"
        border.width: 1
    }

    MouseArea {
        id: extendedKeysMouseArea

        anchors.fill: parent
        preventStealing: true

        /// checks the x value
        // if mouses x is inside the range of popovers x, it looks closer and finds out
        // which section of extended keys is above mouses x value, and selects it
        onPositionChanged:
        {
            var startX = rowOfKeys.x;
            var endX = rowOfKeys.x + __width;

            if (mouse.x > startX && mouse.x < endX) {
                for (var i = 0; i < keyRepeater.count; i++) {

                    // reset highlight for all keys
                    keyRepeater.itemAt(i).highlight = false;

                    if (((startX+keyRepeater.itemAt(i).x) < mouse.x)
                            && ((startX + keyRepeater.itemAt(i).x + keyRepeater.itemAt(i).width) > mouse.x)) {

                        keyRepeater.itemAt(i).highlight = true;
                        __commitStr = keyRepeater.itemAt(i).commitStr;
                    }
                }
            }
        }

        onReleased: {
            __restoreAssignedKey();
            popover.visible = false
            event_handler.onKeyReleased(__commitStr);
        }
    }

    Row {
        id: rowOfKeys
        anchors.centerIn: anchorItem

        Component.onCompleted: __width = 0

        Repeater {
            id: keyRepeater
            model: extendedKeysModel

            Item {
                id: key
                width: textCell.width + units.gu( UI.popoverCellPadding );

                height: panel.keyHeight;

                property alias commitStr: textCell.text
                property bool highlight: false

                Rectangle {
                    anchors.fill: parent
                    color: key.highlight ? "lightGray" : "transparent"
                }

                Text {
                    id: textCell
                    anchors.centerIn: parent;
                    text: modelData
                    font.family: UI.fontFamily
                    font.pixelSize: units.gu( UI.fontSize )
                    font.bold: UI.fontBold
                    color: UI.fontColor
                    Component.onCompleted: __width += (textCell.width + units.gu( UI.popoverCellPadding));
                }

            }
        }
    }

    function enableMouseArea()
    {
        extendedKeysMouseArea.enabled = true
    }

    function __repositionPopoverTo(item)
    {
        var point = keypad.mapFromItem(item.parent, item.x, item.y)

        anchorItem.x = point.x;
        anchorItem.y = point.y - panel.keyHeight;
    }

    function __restoreAssignedKey()
    {
        currentlyAssignedKey.state = "NORMAL"
        currentlyAssignedKey.enableMouseArea();
    }

}


