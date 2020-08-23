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

import "key_constants.js" as UI

/*!
  Item to show a "bubble" with a text in the center.
  The bottom center is where the bubble points to, and it animates to that position
  when hiding it.
 */
KeyPopover {
    id: root

    width: currentlyAssignedKey ? (panel.keyWidth > label.width ? 
                                  panel.keyWidth + 8.0 * (UI.magnifierHorizontalPadding)
                                  : label.width + 8.0 * (UI.magnifierHorizontalPadding)) : 0
    // Use visible key height instead of real key height to allow for bottom
    // row touch area to be extended
    height: currentlyAssignedKey ? panel.keyHeight + 8.0 * (UI.magnifierVerticalPadding) : 0

    /*! Sets the Magnifier visible or invisible*/
    property bool shown: false

    visible: false

    onShownChanged: {
        if (shown) {
            hidePopperAnimation.stop();
            root.visible = true
            popper.animationStep = 1
        } else {
            hidePopperAnimation.start();
        }
    }

    Rectangle {
        id: popper

        width: parent.width
        height: parent.height

        anchors.centerIn: anchorItem

        // this property is used to synchronize scale and opacity animation
        property real animationStep: 0
        scale: animationStep
        transformOrigin: Item.Bottom
        opacity: animationStep

        color: Theme.charKeyColor
        radius: 8 * (0.8)
        border {
            width: 8 * (0.1)
        	color: Theme.popupBorderColor
        }

        onXChanged: {
            if (x < UI.popoverEdgeMargin) {
                anchorItem.x += Math.abs(x) + UI.popoverEdgeMargin;
                return;
            }

            var rightEdge = (x + width);
            if ( rightEdge > (panel.width - UI.popoverEdgeMargin)) {
                var diff = rightEdge - panel.width;
                anchorItem.x -= diff + UI.popoverEdgeMargin;
            }
        }


        Text {
            id: label
            anchors.centerIn: parent
            height: parent.height
            text: currentlyAssignedKey ? currentlyAssignedKey.valueToSubmit : ""
            font.family: UI.fontFamily
            font.weight: Font.Light
            font.pixelSize: panel.keyHeight * 0.6
            verticalAlignment: Text.AlignVCenter

            color: Theme.fontColor
        }

        NumberAnimation {
            id: hidePopperAnimation
            target: popper
            property: "animationStep"
            to: 0
            duration: 50
            easing.type: Easing.InOutQuad
            onStopped: {
                root.visible = false;
            }
        }
    }
}
