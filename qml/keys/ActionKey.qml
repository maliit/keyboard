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

CharKey {
    id: actionKeyRoot
    property string iconNormal: ""
    property string iconShifted: ""
    property string iconCapsLock: ""

    noMagnifier: true
    skipAutoCaps: true
    property int padding: UI.actionKeyPadding

    // action keys are a bit wider
    width: panel.keyWidth + units.gu( padding )

    imgNormal: UI.imageActionKey
    imgPressed: UI.imageActionKeyPressed

    property string __icon: iconNormal

    // can be overwritten by keys
    property color colorNormal: "transparent"
    property color colorShifted: "transparent"
    property color colorCapsLock: "transparent"

    Icon {
        id: iconImage
        name: __icon
        anchors.centerIn: parent
        visible: (label == "")
        width: units.gu(2.5)
        height: units.gu(2.5)
    }

    onOskStateChanged: {
        if (panel.activeKeypadState == "NORMAL") {
            __icon = iconNormal;
            iconImage.color = colorNormal;
        } else if (panel.activeKeypadState == "SHIFTED") {
            __icon = iconShifted;
            iconImage.color = colorShifted;
            console.log(colorShifted)
        } else if (panel.activeKeypadState == "CAPSLOCK") {
            __icon = iconCapsLock;
            iconImage.color = colorCapsLock;
        }
    }
}
