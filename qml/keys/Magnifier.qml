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
import "key_constants.js" as UI

/*!
  Item to show a "bubble" with a text in the center.
  The bottom center is where the bubble points to, and it animates to that position
  when hiding it.
 */
Item {
    id: root

    /*! Text to show in the magnifier */
    property alias text: label.text

    /*! Sets the Magnifier visible or invisible*/
    property bool shown: false

    visible: false

    onShownChanged: {
        if (shown) {
            root.visible = true
            popper.animationStep = 1
        } else {
            hidePopperAnimation.start();
        }
    }

    Image {
        id: popper
        anchors.fill: parent

        // this property is used to synchronize scale and opacity animation
        property real animationStep: 0
        scale: animationStep
        transformOrigin: Item.Bottom
        opacity: animationStep

        source: Qt.resolvedUrl("../styles/ubuntu/images/keyboard_popover.png")

        Text {
            id: label
            anchors.centerIn: parent

            font.family: UI.fontFamily
            font.pixelSize: units.gu( UI.fontSize )
            font.bold: UI.fontBold
            color: UI.fontColor
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
