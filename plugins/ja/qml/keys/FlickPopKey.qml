/*
 * Copyright 2015 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import "key_constants.js" as UI

Rectangle {
    width: 8.0 * (UI.fontSize + UI.flickMargin)
    height: 8.0 * (UI.fontSize + UI.flickMargin)

    property string labelChar
    property color labelColor: UI.fontColor
    property real labelOpacity: 1.0
    visible: labelChar ? true : false

    border.width: 8.0 * (UI.flickBorderWidth)
    border.color: UI.flickBorderColor
    radius: width / 10

    Text {
        anchors.centerIn: parent
        text: parent.labelChar
        font.family: UI.fontFamily
        font.pixelSize: 8.0 * (UI.fontSize)
        font.bold: UI.fontBold
        color: parent.labelColor
        opacity: parent.labelOpacity
    }
}
