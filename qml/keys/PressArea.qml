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

import QtQuick 2.4

import MaliitKeyboard 2.0

/*!
  MultiPointTouchArea is similar to the MouseArea
  But to enable multiple PressAreas to be touched at the same time, this is based
  on MultiPointTouchArea
 */
MultiPointTouchArea {
    id: root

    /// Is true while the area is touched, and the finger did not yet lift
    property bool pressed: false
    // Track whether we've swiped out of a key press to dismiss the keyboard
    property bool swipedOut: false
    property bool horizontalSwipe: false
    property bool held: false
    property alias mouseX: point.x
    property alias mouseY: point.y
    // Keep track of the touch start position ourselves instead of using
    // point.startY, as this always reports 0 for mouse interaction 
    // (https://bugreports.qt.io/browse/QTBUG-41692)
    property real startY

    property bool acceptDoubleClick: false
    maximumTouchPoints: 1

    /// Same as MouseArea pressAndHold()
    signal pressAndHold()

    signal doubleClicked()

    /// Cancels the current pressed state of the mouse are
    function cancelPress() {
        pressed = false;
        holdTimer.stop();
    }

    enabled: MaliitGeometry.shown

    touchPoints: [
        TouchPoint { 
            id: point
            property double lastY
            property double lastYChange

            // Dragging implemented here rather than in higher level
            // mouse area to avoid conflict with swipe selection
            // of extended keys
            onYChanged: {
                if (point.y > root.y + root.height) {
                    if (!swipedOut) {
                        // We've swiped out of the key
                        swipedOut = true;
                        cancelPress();
                    }
                } else {
                    lastY = point.y;
                }
            }
        }
    ]

    Timer {
        id: holdTimer
        interval: 300
        onTriggered: {
            if (root.pressed) {
                root.pressAndHold();
                held = true;
            }
        }
    }

    Timer {
        id: doubleClickTimer
        interval: 400 // Default Qt double click interval
    }

    onPressed: {
        pressed = true;
        held = false;
        swipedOut = false;
        startY = point.y;
        holdTimer.restart();

        // We keep a global view of whether any other keys have been
        // pressed in between the first tap on this key and the second
        // one, so that quickly tapping one key, then tapping another,
        // then tapping the first again doesn't trigger a double click.

        if (doubleClickTimer.running) {
            if (panel.lastKeyPressed == root) {
                doubleClicked();
            }
        } else {
            if (acceptDoubleClick) {
                doubleClickTimer.restart();
            }
            panel.lastKeyPressed = root
        }
    }

    onReleased: {
        pressed = false;
        held = false;
        holdTimer.stop();
    }
}
