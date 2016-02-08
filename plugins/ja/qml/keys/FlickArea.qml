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

MultiPointTouchArea {
    id: root

    /// Is true while the area is touched, and the finger did not yet lift
    property bool pressed: false

    /// Cancels the current pressed state of the mouse are
    function cancelPress() {
        pressed = false;
    }

    onPressed: {
        index = 0;
        pressed = true;
        rad = 0
    }

    onReleased: {
        pressed = false;
    }

    property int index: 0   // 0:center, 1:left, 2:top, 3:right, 4:bottom
    property int old_index: 0
    property real posX: point.x - parent.width / 2
    property real posY: point.y - parent.height / 2
    property real rad: 0

    touchPoints: [
        TouchPoint { id: point }
    ]

    onUpdated: {
        rad = Math.atan2(posY, posX)
        if ((posX * posX + posY * posY) < (0.5 * parent.height * 0.5 * parent.height)) {
            index = 0
        } else {
            if (rad < -Math.PI / 4.0) {
                if (rad < -Math.PI * 3.0 / 4.0) {
                    index = 1
                } else {
                    index = 2
                }
            } else if (rad > Math.PI / 4.0) {
                if (rad > Math.PI * 3.0 / 4.0) {
                    index = 1
                } else {
                    index = 4
                }
            } else {
                index = 3
            }
        }

        if (old_index != index) {
            old_index = index

            if (maliit_input_method.useHapticFeedback)
                pressEffect.start();
        }
    }
}
