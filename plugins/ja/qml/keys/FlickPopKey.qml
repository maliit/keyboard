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

import MaliitKeyboard 2.0

Rectangle {
    width: Device.fontSize + Device.flickMargin
    height: Device.fontSize + Device.flickMargin

    property string labelChar
    property color labelColor: Theme.fontColor
    property real labelOpacity: 1.0
    visible: labelChar ? true : false

    border.width: Device.flickBorderWidth
    border.color: "#888888"
    radius: width / 10

    Text {
        anchors.centerIn: parent
        text: parent.labelChar
        font.family: Theme.fontFamily
        font.pixelSize: Device.fontSize
        font.bold: Device.fontBold
        color: parent.labelColor
        opacity: parent.labelOpacity
    }
}
