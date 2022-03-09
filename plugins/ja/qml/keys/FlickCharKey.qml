/*
 * Copyright 2015 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import QtQuick.Controls 2.12

import MaliitKeyboard 2.0

Item {
    id: key

    property int padding: 0

    width: leftSide || rightSide ? panel.keyWidth * 2 : panel.keyWidth
    height: panel.keyHeight

    /* to be set in keyboard layouts */
    property string label: "";
    property var leaves: ["", "", "", "", ""];
    property int index: keyFlickArea.index;
    property bool highlight: false;

    property string action
    property bool noMagnifier: !Keyboard.enableMagnifier
    property bool skipAutoCaps: false
    property bool switchBackFromSymbols: false

    property bool leftSide: false
    property bool rightSide: false

    property double rowMargin: fullScreenItem.landscape ? Device.rowMarginLandscape
                                                        : Device.rowMarginPortrait
    property double keyMargin: Device.keyMargins

    // These properties are used by autopilot to determine the visible
    // portion of the key to press
    readonly property double leftOffset: buttonRect.anchors.leftMargin
    readonly property double rightOffset: buttonRect.anchors.rightMargin

    /* design */
    property int fontSize: (fullScreenItem.landscape ? (height / 2) : (height / 2.8));

    /// annotation shows a small label in the upper right corner
    // if the annotiation property is set, it will be used. If not, the first position in extended[] list or extendedShifted[] list will
    // be used, depending on the state. If no extended/extendedShifted arrays exist, no annotation is shown
    property string annotation: ""

    /*! indicates if te key is currently pressed/down*/
    property alias currentlyPressed: keyFlickArea.pressed

    property string oskState: panel.activeKeypadState

    // Allow action keys to override the standard key behaviour
    property bool overridePressArea: false

    signal pressed()
    signal released()

    // Make it possible for the visible area of the key to differ from the
    // actual key size. This allows us to extend the touch area of the bottom
    // row of keys all the way to the bottom of the keyboard, whilst
    // maintaining the same visual appearance.
    Item {
        anchors.top: parent.top
        height: panel.keyHeight
        width: parent.width

        ToolButton {
            id: buttonRect
            anchors.fill: parent
            anchors.leftMargin: key.leftSide ? (parent.width - panel.keyWidth) + key.keyMargin : key.keyMargin
            anchors.rightMargin: key.rightSide ? (parent.width - panel.keyWidth) + key.keyMargin : key.keyMargin
            anchors.bottomMargin: key.rowMargin

            /// label of the key
            //  the label is also the value subitted to the app

            Column {
                spacing: Device.gu(0.3)
                anchors.centerIn: parent

                Label {
                    id: keyLabel
                    text: label
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: key.fontSize
                    font.weight: Font.Light
                    textFormat: Text.StyledText
                }

                Label {
                    id: annotationLabel
                    text: annotation

                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottomMargin: Device.gu(0.3)

                    font.pixelSize: Device.annotationFontSize
                    font.weight: Font.Light
                    visible: annotation != ""
                }
            }
        }

        FlickPop {
            anchors.horizontalCenter: buttonRect.horizontalCenter
            anchors.bottom: buttonRect.top
            anchors.bottomMargin: key.height * 0.5
            width: Device.fontSize + Device.flickMargin * 3
            height: Device.fontSize + Device.flickMargin * 3
            chars: leaves
            index: keyFlickArea.index
            visible: key.currentlyPressed && chars.length > 1 && !noMagnifier
        }
    }

    FlickArea {
        id: keyFlickArea
        anchors.fill: key

        onReleased: {
            if (overridePressArea) {
                key.released();
                return;
            }

            event_handler.onKeyReleased(leaves[index], action);
        }

        onPressed: {
            if (overridePressArea) {
                key.pressed();
                return;
            }

            Feedback.keyPressed();

            event_handler.onKeyPressed(leaves[index], action);
        }
    }

    Connections {
        target: swipeArea.drag
        onActiveChanged: {
            if (swipeArea.drag.active)
                keyFlickArea.cancelPress();
        }
    }
}
