/*
 * This file is part of Maliit plugins
 *
 * Copyright (C) 2012 Openismus GmbH
 *
 * Contact: maliit-discuss@lists.maliit.org
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Nokia Corporation nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

import QtQuick 2.0
import "constants.js" as Const
import "keys/"
import "keys/key_constants.js" as UI
import Ubuntu.Components 0.1
import QtQuick.Window 2.0

Item {
    id: fullScreenItem
    objectName: "fullScreenItem"

    property variant input_method: maliit_input_method
    property variant layout: maliit_layout
    property variant event_handler: maliit_event_handler

    onHeightChanged: calculateSize();

OrientationHelper {
    id: orientationHelper
    automaticOrientation: false
    transitionEnabled: false

    orientationAngle: Screen.angleBetween(Screen.primaryOrientation, canvas.contentOrientation);

    onOrientationAngleChanged: {
        calculateSize();
        fullScreenItem.reportKeyboardVisibleRect();
    }
    onXChanged: fullScreenItem.reportKeyboardVisibleRect();
    onYChanged: fullScreenItem.reportKeyboardVisibleRect();
    onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

Item {
    id: canvas
    objectName: "ubuntuKeyboard" // Allow us to specify a specific keyboard within autopilot.

    anchors.bottom: parent.bottom
    anchors.left: parent.left

    width: parent.width
    height: 0

    property int keypadHeight: height;

    onRotationChanged: console.log("now rotation has changed!!" + rotation)

    visible: true

    // Expose details for use with Autopilot.
    //readonly property var layoutState: layout.keyboard_state
    //readonly property string activeView: layout.activeView

    property int contentOrientation: maliit_geometry.orientation
    onContentOrientationChanged: fullScreenItem.reportKeyboardVisibleRect();

    property bool wordribbon_visible: maliit_input_method.showWordRibbon

    property bool languageMenuShown: false

    onXChanged: fullScreenItem.reportKeyboardVisibleRect();
    onYChanged: fullScreenItem.reportKeyboardVisibleRect();
    onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
    onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

    MouseArea {
        id: swipeArea

        property int jumpBackThreshold: units.gu(10)

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: (parent.height - canvas.keypadHeight) + wordRibbon.height +
                borderTop.height + units.gu(UI.top_margin) * 3

        drag.target: keyboardSurface
        drag.axis: Drag.YAxis;
        drag.minimumY: 0
        drag.maximumY: parent.height
        drag.filterChildren: true

        onReleased: {
            if (keyboardSurface.y > jumpBackThreshold) {
                maliit_geometry.shown = false;
            } else {
                bounceBackAnimation.from = keyboardSurface.y
                bounceBackAnimation.start();
            }
        }

        Item {
            id: keyboardSurface
            objectName: "keyboardSurface"

            x:0
            y:0
            width: parent.width
            height: canvas.height

            onXChanged: fullScreenItem.reportKeyboardVisibleRect();
            onYChanged: fullScreenItem.reportKeyboardVisibleRect();
            onWidthChanged: fullScreenItem.reportKeyboardVisibleRect();
            onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

            WordRibbon {
                id: wordRibbon
                objectName: "wordRibbon"

                visible: canvas.wordribbon_visible

                anchors.bottom: keyboardComp.top
                width: parent.width;

                height: visible ? UI.wordribbon_height : 0
                onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();
            }

            Item {
                id: keyboardComp
                objectName: "keyboardComp"

                height: canvas.keypadHeight - wordRibbon.height
                width: parent.width
                anchors.bottom: parent.bottom

                onHeightChanged: fullScreenItem.reportKeyboardVisibleRect();

                Rectangle {
                    id: background

                    anchors.fill: parent

                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#f1f1f1" }
                        GradientStop { position: 1.0; color: "#e4e4e4" }
                    }
                }

                Image {
                    id: borderTop
                    source: "styles/ubuntu/images/border_top.png"
                    width: parent.width
                    anchors.top: parent.top.bottom
                }

                Image {
                    id: borderBottom
                    source: "styles/ubuntu/images/border_bottom.png"
                    width: parent.width
                    anchors.bottom: background.bottom
                }

                KeyboardContainer {
                    id: keypad

                    anchors.top: borderTop.bottom
                    anchors.bottom: borderBottom.top
                    anchors.topMargin: units.gu( UI.top_margin )
                    anchors.bottomMargin: units.gu( UI.bottom_margin )
                    width: parent.width

                    onPopoverEnabledChanged: fullScreenItem.reportKeyboardVisibleRect();
                }

                LanguageMenu {
                    id: languageMenu
                    anchors.centerIn: parent
                    width: 400;
                    height: keypad.height;
                    enabled: canvas.languageMenuShown
                    opacity: canvas.languageMenuShown ? 1.0 : 0.0
                }
            } // keyboardComp
        }
    }

    PropertyAnimation {
        id: bounceBackAnimation
        target: keyboardSurface
        properties: "y"
        easing.type: Easing.OutBounce;
        easing.overshoot: 2.0
        to: 0
    }

    state: "HIDDEN"

    states: [
        State {
            name: "SHOWN"
            PropertyChanges { target: canvas; y: 0; }
            when: maliit_geometry.shown === true
        },

        State {
            name: "HIDDEN"
            PropertyChanges { target: canvas; y: height; }
            onCompleted: {
                canvas.languageMenuShown = false;
                keyboardSurface.y = 0;
                keypad.closeExtendedKeys();
                keypad.activeKeypadState = "NORMAL";
                keypad.state = "CHARACTERS";
                maliit_input_method.hide();
            }
            when: maliit_geometry.shown === false
        }
    ]
    transitions: Transition {
        PropertyAnimation { target: canvas; properties: "y"; easing.type: Easing.InOutQuad }
    }

    Connections {
        target: input_method
        onActivateAutocaps: {
            keypad.state = "CHARACTERS";
            keypad.activeKeypadState = "SHIFTED";
        }
    }

} // canvas
} // OrientationHelper

function calculateSize()
{
    // TODO tablet
    if (orientationHelper.orientationAngle == 270) {
        console.log("LANDSCAPE");
        canvas.height = (fullScreenItem.width * UI.phone_keyboard_height_landscape) + wordRibbon.height
    }
    else if (orientationHelper.orientationAngle == 0) {
        console.log("PORTRAIT");
        canvas.height = (fullScreenItem.height * UI.phone_keyboard_height_portrait) + wordRibbon.height
    }

    // fallback
    else {
        canvas.height = (fullScreenItem.height * UI.phone_keyboard_height_portrait) + wordRibbon.height
    }
//    console.log("calc height " + canvas.height + fullScreenItem.height)
    reportKeyboardVisibleRect();
}

// calculates the size of the visible keyboard to report to the window system
// FIXME get the correct size for enabled extended keys instead of that big area
function reportKeyboardVisibleRect() {

    var vx = 0;
    var vy = wordRibbon.y;
    var vwidth = keyboardSurface.width;
    var vheight = keyboardComp.height + wordRibbon.height;
    if (!canvas.wordribbon_visible && keypad.popoverEnabled) {
        vy = 0;
        vheight = keyboardSurface.height;
    }

    var obj = mapFromItem(keyboardSurface, vx, vy, vwidth, vheight);
    maliit_geometry.visibleRect = Qt.rect(obj.x, obj.y, obj.width, obj.height);
}

} // fullScreenItem
