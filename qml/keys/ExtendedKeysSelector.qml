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

import "key_constants.js" as UI

Popover {

    id: popover

    property variant extendedKeysModel

    Rectangle {
        anchors.fill: containerLayout
        color: "white"
    }

    Row {
        id: containerLayout
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }

        Repeater {
            model: extendedKeysModel

            Item {
                width: units.gu( UI.popoverCellSize );
                height: panel.keyHeight;

                Text {
                    anchors.centerIn: parent;
                    text: modelData
                    font.family: UI.fontFamily
                    font.pixelSize: fontSize
                    font.bold: UI.fontBold
                    color: UI.fontColor
                }

                MouseArea {
                    anchors.fill: parent
                    onReleased: {
                        event_handler.onKeyReleased(modelData);
                        popover.hide();
                    }
                }
            }
        }
    }
}


