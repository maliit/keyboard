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

    property alias mouseArea: dismissMouseArea

    visible: false

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
        id: background

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

    MouseArea {
        id: dismissMouseArea

        anchors.fill: parent
        propagateComposedEvents: true
        onClicked: popover.visible = false
        preventStealing: true

        onPositionChanged:
        {
            var startX = containerLayout.x
            var endX = containerLayout.x + __width

            if (mouse.x > startX && mouse.x < endX) {
                for (var i = 0; i < layoutRepeater.count; i++) {

                    layoutRepeater.itemAt(i).highlight = false

                    if (((startX+layoutRepeater.itemAt(i).x) < mouse.x)
                            && ((startX + layoutRepeater.itemAt(i).x + layoutRepeater.itemAt(i).width) > mouse.x)) {
                        layoutRepeater.itemAt(i).highlight = true
                        __commitStr = layoutRepeater.itemAt(i).txt
                    }
                }
            }
        }

        onReleased: {
            currentlyAssignedKey.state = "NORMAL"
            popover.visible = false
            event_handler.onKeyReleased(__commitStr);
        }
    }

    property int __width: 0
    property string __commitStr: ""

    Row {
        id: containerLayout
        anchors.centerIn: anchorItem

        Component.onCompleted: __width = 0

        Repeater {
            id: layoutRepeater
            model: extendedKeysModel

            Item {
                id: itemContainer
                width: textCell.width + units.gu( UI.popoverCellPadding );

                height: panel.keyHeight;

                property alias txt: textCell.text
                property bool highlight: false

                Rectangle {
                    anchors.fill: parent
                    color: itemContainer.highlight ? "red" : "#00000000"
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

                MouseArea {
                    anchors.fill: parent
                    enabled: true
                    visible: true
                    onReleased: {
                        event_handler.onKeyReleased(modelData);
                        popover.visible = false;
                    }

                    preventStealing: true
                }

            }
        }
    }
}


