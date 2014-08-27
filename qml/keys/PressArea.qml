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
    // Track whether we've swiped out of a key press to dismiss the keyboard
    property bool swipedOut: false
    property bool held: false
    property alias mouseX: point.x
    property alias mouseY: point.y

    property bool acceptDoubleClick: false

    /// Same as MouseArea pressAndHold()
    signal pressAndHold()

    signal doubleClicked()

    /// Cancels the current pressed state of the mouse are
    function cancelPress() {
        pressed = false;
        holdTimer.stop();
    }

    touchPoints: [
        TouchPoint { 
            id: point
            property double lastY
            property double lastChange
            onYChanged: {
                if (point.y > root.y + root.height) {
                    if (!swipedOut) {
                        // We've swiped out of the key
                        swipedOut = true;
                        cancelPress();
                    }

                    // Dragging implemented here rather than in higher level
                    // mouse area to avoid conflict with swipe selection
                    // of extended keys
                    var distance = point.y - lastY;
                    // If changing direction wait until movement passes 1 gu
                    // to avoid jitter
                    if ((lastChange * distance > 0 || Math.abs(distance) > units.gu(1)) && !held) {
                        keyboardSurface.y += distance;
                        lastY = point.y;
                        lastChange = distance;
                    }
                    // Hide if we get close to the bottom of the screen
                    // This works around issues with devices with touch buttons
                    // below the screen preventing release events when swiped
                    // over
                    if(point.sceneY > fullScreenItem.height - units.gu(4) && point.y > point.startY + units.gu(8) && !held) {
                        maliit_input_method.hide();
                    }
                } else {
                    lastY = point.y;
                }
            }
        }
    ]

    Timer {
        id: holdTimer
        interval: 1000
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
        holdTimer.restart();
        if (doubleClickTimer.running) {
            doubleClicked();
        }
        if (acceptDoubleClick) {
            doubleClickTimer.restart();
        }
    }

    onReleased: {
        // Allow the user to swipe away the keyboard
        if (point.y > point.startY + units.gu(8) && !held) {
            maliit_input_method.hide();
        } else {
            bounceBackAnimation.from = keyboardSurface.y;
            bounceBackAnimation.start();
        }
        pressed = false;
        held = false;
        holdTimer.stop();
    }
}
