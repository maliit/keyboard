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

    property variant extendedKeysModel

    property Item currentlyAssignedKey

    visible: false

    Rectangle {
        id: dismissArea

        anchors.fill: parent
        anchors.centerIn: popover
        color: "#66FFF000"

        MouseArea {
            anchors.fill: parent
            onClicked: popover.visible = false
        }
    }

    onCurrentlyAssignedKeyChanged:
    {
        if (currentlyAssignedKey == null)
            return;

        var point = keypad.mapFromItem(currentlyAssignedKey.parent, currentlyAssignedKey.x, currentlyAssignedKey.y)

        anchorItem.x = point.x;
        anchorItem.y = point.y - panel.keyHeight;
    }

    Item {
        id: anchorItem
        width: panel.keyWidth
        height: panel.keyHeight
    }

    Rectangle {
        id: name

        anchors.centerIn: anchorItem
        width: {
            if (containerLayout.width < keypad.keyWidth)
                return keypad.keyWidth;
            else
                return containerLayout.width;
        }

        height: containerLayout.height
        radius: 17
        color: "white"
        border.color: "lightGray"
        border.width: 1
    }


    Row {
        id: containerLayout
        anchors.centerIn: anchorItem

        Repeater {
            model: extendedKeysModel

            Item {
                width: textCell.width + units.gu( UI.popoverCellPadding );
                height: panel.keyHeight;

                Text {
                    id: textCell
                    anchors.centerIn: parent;
                    text: modelData
                    font.family: UI.fontFamily
                    font.pixelSize: units.gu( UI.fontSize )
                    font.bold: UI.fontBold
                    color: UI.fontColor
                }

                MouseArea {
                    anchors.fill: parent
                    onReleased: {
                        event_handler.onKeyReleased(modelData);
                        popover.visible = false;
                    }
                }
            }
        }
    }
}


