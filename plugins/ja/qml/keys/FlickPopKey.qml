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
import QtQuick.Controls 2.12

import MaliitKeyboard 2.0

Rectangle {
    id: popkey

    width: Device.fontSize + Device.flickMargin
    height: Device.fontSize + Device.flickMargin

    property string labelChar
    property bool selected: false
    property real labelOpacity: 1.0
    visible: labelChar ? true : false

    // Invisible tooltip to get popup colors
    ToolTip {
        id: tip
        visible: false
    }

    color: tip.background.color
    border.width: Device.flickBorderWidth
    border.color: tip.background.border.color
    radius: width / 10

    // Invisible text field to get selection colors
    TextField {
        id: textArea
        width: 0
        height: 0
        visible: false
    }

    Label {
        anchors.centerIn: parent
        text: parent.labelChar
        font.pixelSize: Device.fontSize
        font.bold: Device.fontBold
        color: popkey.selected ? textArea.selectionColor : textArea.color
        opacity: parent.labelOpacity
    }
}
