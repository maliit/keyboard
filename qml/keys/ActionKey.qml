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
    property string action: ""

    // TODO implement sizes S,M,L,XL,XXL in C++ enums
    //Component.onCompleted: width = width*1.2;

    imgNormal: UI.imageActionKey
    imgPressed: UI.imageActionKeyPressed

    Image {
        id: iconImage
        source: iconNormal
        anchors.centerIn: parent
    }

    onOskStateChanged: {
        if (panel.activeKeypad.state == "NORMAL")
            iconImage.source = iconNormal;
        if (panel.activeKeypad.state == "SHIFTED")
            iconImage.source = iconShifted;
    }
}
