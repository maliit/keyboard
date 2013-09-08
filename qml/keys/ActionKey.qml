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

import "key_constants.js" as UI

CharKey {
    id: actionKeyRoot
    property string iconNormal: ""
    property string iconShifted: ""
    property string iconCapsLock: ""

    noMagnifier: true
    property int padding: UI.actionKeyPadding

    // action keys are a bit wider
    width: panel.keyWidth + units.gu( padding )

    imgNormal: UI.imageActionKey
    imgPressed: UI.imageActionKeyPressed

    Image {
        id: iconImage
        source: iconNormal
        anchors.centerIn: parent
        visible: (label == "")
    }

    onOskStateChanged: {
        if (panel.activeKeypad.state == "NORMAL")
            iconImage.source = iconNormal;
        if (panel.activeKeypad.state == "SHIFTED")
            iconImage.source = iconShifted;
        if (panel.activeKeypad.state == "CAPSLOCK")
            iconImage.source = iconCapsLock
    }
}
