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

Item {
    id: canvas
    property alias layout: keyRepeater.model
    property variant event_handler
    property bool area_enabled // MouseArea has no id property so we cannot alias its enabled property.
    property alias title: keyboard_title.text

    visible: layout.visible

    property int contentOrientation: Qt.PrimaryOrientation

    property bool shown: false;
    property bool wordribbon_visible: true;

    property bool hideAnimationFinished: false;
    property int pressedKeyIndex: -1;
    property Item pressedKey;

    RotationHelper {

    Connections {
        target: layout
        onTitleChanged: {
            console.debug("title:" + layout.title)
            title_timeout.start()
        }
    }

    MouseArea {
        id: keyboardSurface
        property int jumpBackThreshold: 170

        drag.target: keyboardSurface
        drag.axis: Drag.YAxis;
        drag.minimumY: 0
        drag.maximumY: height
        drag.filterChildren: true

        x:0
        y:0
        width: parent.width
        height: parent.height


    WordRibbon {
        id: wordRibbon

        anchors.bottom: keypadMouseArea.top
        width: parent.width;

        height: wordribbon_visible ? layout.wordribbon_height : 0
    }

    MouseArea {
        id: keypadMouseArea
        preventStealing: true

        anchors {
            top: wordRibbon.bottom
            fill: parent
            topMargin: layout.invisible_toucharea_height + (wordribbon_visible ? layout.wordribbon_height : 0);
        }

        Item {
            id: keyboardContainer

            anchors.fill: parent

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
                anchors.top: borderTop.bottom
                anchors.bottom: borderBottom.top
                width: parent.width
            }

            Item {
                id: keyPad
                visible: false;
                anchors.top: borderTop.bottom
                anchors.bottom: borderBottom.top
                width: parent.width

            Repeater {
                id: keyRepeater
                model: layout
                anchors.fill: parent

                Item {
                    property alias text: key_text_item.text;

                    x: key_reactive_area.x
                    y: key_reactive_area.y
                    width: key_reactive_area.width
                    height: key_reactive_area.height

                    BorderImage {
                        x: key_rectangle.x
                        y: key_rectangle.y
                        width: key_rectangle.width
                        height: key_rectangle.height

                        border.left: key_background_borders.x
                        border.top: key_background_borders.y
                        border.right: key_background_borders.width
                        border.bottom: key_background_borders.height

                        source: key_background

                        Text {
                            id: key_text_item

                            anchors.fill: parent
                            text: key_text
                            font.family: key_font
                            font.pixelSize: key_font_size
                            color: key_font_color
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            visible: (key_text.length != 0)
                        }

                        Image {
                            anchors.centerIn: parent
                            source: key_icon
                            visible: (key_icon.length != 0)
                        }
                    }

                    MouseArea {
                        enabled: area_enabled
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: event_handler.onEntered(index)
                        onExited: event_handler.onExited(index)

                        onPressed: {
                            if(!drag.active) {
                                pressedKeyIndex = index;
                                event_handler.onEntered(index)
                            }

                            if (key_action_insert)
                                pressedKey = parent
                            else
                                pressedKey = null

                            event_handler.onPressed(index)
                            mouse.accepted = false;
                        }

                        onReleased: {
                            console.error("ON_RELEASED")
                            if (pressedKeyIndex == -1)
                                return;

                            if (!drag.active)
                                event_handler.onReleased(pressedKeyIndex);

                            event_handler.onExited(pressedKeyIndex)
                            pressedKeyIndex = -1;

                        }

                        onPressAndHold: event_handler.onPressAndHold(index)
                    }

                } // Item
            } // Repeater

            } // keyPad

            Popper {
                id: popper
                target: pressedKey
            }

            // Keyboard title rendering
            // TODO: Make separate component?
            Item {
                anchors.centerIn: parent
                opacity: title_timeout.running ? 1.0 : 0.0

                Behavior on opacity {
                    PropertyAnimation {
                        duration: 300
                        easing.type: Easing.InOutQuad
                    }
                }

                Timer {
                    id: title_timeout
                    interval: 1000
                }

                // TODO: Make title background part of styling profile.
                BorderImage {
                    anchors.centerIn: parent

                    // Manual padding of text:
                    width: keyboard_title.width * 1.2
                    height: keyboard_title.height * 1.2

                    //anchors.fill: keyboard_title
                    source: layout.background
                    z: 1000 // Move behind Text element but in front of rest.

                    border.left: layout.background_borders.x
                    border.top: layout.background_borders.y
                    border.right: layout.background_borders.width
                    border.bottom: layout.background_borders.height
                }

                Text {
                    id: keyboard_title
                    anchors.centerIn: parent

                    text: title;
                    z: 1001

                    // TODO: Make title font part of styling profile.
                    font.pointSize: 48
                    color: "white"
                }
            }
        } // keyboardContainer

        onReleased: {
            if (pressedKeyIndex == -1)
                return;

            if (!drag.active)
                event_handler.onReleased(pressedKeyIndex);

            event_handler.onExited(pressedKeyIndex)
            pressedKeyIndex = -1;
            pressedKey = null;

        }
    } // keypadMouseArea

    onReleased: {
        if (y > jumpBackThreshold) {
            canvas.shown = false;
        } else {
            bounceBackAnimation.from = y
            bounceBackAnimation.start();
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

    } // big mousearea
    } // rotation helper

    state: "HIDDEN"

    states: [
        State {
            name: "SHOWN"
            PropertyChanges { target: canvas; y: 0; }
            when: canvas.shown === true
            onCompleted: canvas.hideAnimationFinished = false;
        },

        State {
            name: "HIDDEN"
            PropertyChanges { target: canvas; y: height; }
            onCompleted: {
                keyboardSurface.y = 0;
                canvas.hideAnimationFinished = true;
            }
            when: canvas.shown === false
        }
    ]
    transitions: Transition {
        PropertyAnimation { target: canvas; properties: "y"; easing.type: Easing.InOutQuad }
    }
}

