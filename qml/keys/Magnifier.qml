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
import QtQuick.Controls 2.12

import MaliitKeyboard 2.0

/*!
  Item to show a "bubble" with a text in the center.
  The bottom center is where the bubble points to, and it animates to that position
  when hiding it.
 */
KeyPopover {
    id: root

    width: panel.keyWidth
    height: panel.keyHeight * 2

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

        //fix anchor loop, see https://forum.qt.io/topic/103522/anchor-loop-with-layout-centerin/4
        anchors.horizontalCenter: anchorItem.horizontalCenter
        anchors.verticalCenter: anchorItem.verticalCenter

        // this property is used to synchronize scale and opacity animation
        property real animationStep: 0
        scale: animationStep
        transformOrigin: Item.Bottom
        opacity: animationStep

        // Invisible tooltip to copy the qqc2 style colors from
        ToolTip {
            id: tip
            visible: false
        }

        color: tip.background.color
        radius: 8 * (0.8)
        border {
            width: tip.background.border.width
            color: tip.background.border.color
        }

        onXChanged: {
            if (x < Device.popoverEdgeMargin) {
                anchorItem.x += Math.abs(x) + Device.popoverEdgeMargin;
                return;
            }

            var rightEdge = (x + width);
            if ( rightEdge > (panel.width - Device.popoverEdgeMargin)) {
                var diff = rightEdge - panel.width;
                anchorItem.x -= diff + Device.popoverEdgeMargin;
            }
        }


        Label {
            id: label
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            text: currentlyAssignedKey ? currentlyAssignedKey.valueToSubmit : ""
            font.weight: Font.Light
            font.pixelSize: panel.keyHeight * 0.6
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
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
