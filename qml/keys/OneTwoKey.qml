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

ActionKey {
    iconNormal: "../images/icon_shift@18.png";
    iconShifted: "../images/icon_shift_upsidedown@18.png";
    iconCapsLock: "../images/icon_shift_white@18.png";
    action: "shift"

    MouseArea {
        anchors.fill: parent
        preventStealing: true
        onClicked: {
            if (panel.activeKeypadState == "NORMAL")
                panel.activeKeypadState = "SHIFTED";

            else if (panel.activeKeypadState == "SHIFTED")
                panel.activeKeypadState = "NORMAL"

            else if (panel.activeKeypadState == "CAPSLOCK")
                panel.activeKeypadState = "NORMAL"
        }
    }
}
