/*
 * Copyright 2013 Canonical Ltd.
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

import QtQuick 2.0

/*!
  MultiPointTouchArea is similar to the MouseArea
  But to enable multiple PressAreas to be touched at the same time, this is based
  on MultiPointTouchArea

  FIXME this compoment assumes, that only one finger touches this single area at
  the same time.
 */
MultiPointTouchArea {
    id: root

    /// Is true while the area is touched, and the finger did not yet lift
    property bool pressed: false

    /// Same as MouseArea pressAndHold()
    signal pressAndHold()

    /// Cancels the current pressed state of the mouse are
    function cancelPress() {
        pressed = false;
        holdTimer.stop();
    }

    Timer {
        id: holdTimer
        interval: 1000
        onTriggered: {
            if (root.pressed)
                root.pressAndHold();
        }
    }

    onPressed: {
        pressed = true;
        holdTimer.restart();
    }

    onReleased: {
        pressed = false;
        holdTimer.stop();
    }
}
