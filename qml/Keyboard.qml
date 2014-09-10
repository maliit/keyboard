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
    property variant event_handler: maliit_event_handler

    onHeightChanged: calculateSize();

    OrientationHelper {
        id: orientationHelper
        automaticOrientation: false
        transitionEnabled: false

        orientationAngle: Screen.angleBetween(Screen.primaryOrientation, canvas.contentOrientation);

        onOrientationAngleChanged: {
            calculateSize();
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

            property int contentOrientation: maliit_geometry.orientation
            onContentOrientationChanged: fullScreenItem.reportKeyboardVisibleRect();

            property bool wordribbon_visible: maliit_word_engine.enabled
            onWordribbon_visibleChanged: calculateSize();

            property bool languageMenuShown: false
            property bool extendedKeysShown: false

            property bool firstShow: true

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
                borderTop.height + units.gu(UI.top_margin)

                drag.target: keyboardSurface
                drag.axis: Drag.YAxis;
                drag.minimumY: 0
                drag.maximumY: parent.height
                //fix for lp:1277186
                //only filter children when wordRibbon visible
                drag.filterChildren: wordRibbon.visible
                // Avoid conflict with extended key swipe selection
                enabled: !canvas.extendedKeysShown

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

                        height: canvas.wordribbon_visible ? units.gu(UI.wordribbonHeight) : 0
                        onHeightChanged: calculateSize();
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
                    PropertyChanges { target: keyboardSurface; y: 0; }
                    onCompleted: {
                        canvas.firstShow = false;
                    }
                    when: maliit_geometry.shown === true
                },

                State {
                    name: "HIDDEN"
                    PropertyChanges { target: keyboardSurface; y: canvas.height }
                    onCompleted: {
                        canvas.languageMenuShown = false;
                        keypad.closeExtendedKeys();
                        keypad.activeKeypadState = "NORMAL";
                        keypad.state = "CHARACTERS";
                        maliit_input_method.close();
                    }
                    // Wait for the first show operation to complete before
                    // allowing hiding, as the conditions when the keyboard
                    // has never been visible can trigger a hide operation
                    when: maliit_geometry.shown === false && canvas.firstShow === false
                }
            ]
            transitions: Transition {
                UbuntuNumberAnimation { target: keyboardSurface; properties: "y"; }
            }

            Connections {
                target: input_method
                onActivateAutocaps: {
                    if (keypad.state == "CHARACTERS") {
                        keypad.activeKeypadState = "SHIFTED";
                        keypad.autoCapsTriggered = true;
                    } else {
                        keypad.delayedAutoCaps = true;
                    }
                }

                onKeyboardReset: {
                    keypad.state = "CHARACTERS"
                }
            }

        } // canvas
    } // OrientationHelper

    function calculateSize() {
        // TODO tablet
        if (orientationHelper.orientationAngle == 270 ||
            orientationHelper.orientationAngle == 90) {
            canvas.height = (fullScreenItem.width * UI.phoneKeyboardHeightLandscape) + wordRibbon.height
        } else if (orientationHelper.orientationAngle == 0 ||
                   orientationHelper.orientationAngle == 180) {
            canvas.height = (fullScreenItem.height * UI.phoneKeyboardHeightPortrait) + wordRibbon.height
        } else { // fallback
            canvas.height = (fullScreenItem.height * UI.phoneKeyboardHeightPortrait) + wordRibbon.height
        }

        reportKeyboardVisibleRect();
    }

    function reportKeyboardVisibleRect() {

        var vx = 0;
        var vy = wordRibbon.y;
        var vwidth = keyboardSurface.width;
        var vheight = keyboardComp.height + wordRibbon.height;

        var obj = mapFromItem(keyboardSurface, vx, vy, vwidth, vheight);
        // Report visible height of the keyboard to support anchorToKeyboard
        obj.height = fullScreenItem.height - obj.y;
        maliit_geometry.visibleRect = Qt.rect(obj.x, obj.y, obj.width, obj.height);
    }

} // fullScreenItem
