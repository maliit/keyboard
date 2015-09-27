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

import QtQuick 2.0
import QtMultimedia 5.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1

import "key_constants.js" as UI

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
    property bool noMagnifier: false
    property bool skipAutoCaps: false
    property bool switchBackFromSymbols: false

    property bool leftSide: false
    property bool rightSide: false

    // These properties are used by autopilot to determine the visible
    // portion of the key to press
    readonly property double leftOffset: buttonImage.anchors.leftMargin
    readonly property double rightOffset: buttonImage.anchors.rightMargin

    /* design */
    property string imgNormal: UI.imageCharKey
    property string imgPressed: UI.imageCharKeyPressed
    // fontSize can be overwritten when using the component, e.g. SymbolShiftKey uses smaller fontSize
    property int fontSize: units.gu( UI.fontSize );

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

        BorderImage {
            id: buttonImage
            anchors.fill: parent
            anchors.leftMargin: key.leftSide ? (parent.width - panel.keyWidth) + units.dp(UI.keyMargins) :  units.dp(UI.keyMargins)
            anchors.rightMargin: key.rightSide ? (parent.width - panel.keyWidth) + units.dp(UI.keyMargins) :  units.dp(UI.keyMargins)
            anchors.bottomMargin: fullScreenItem.landscape ? units.dp( UI.keyMargins ) * 2 : units.gu(UI.row_margin);
            source: key.imgNormal

            BorderImage {
                anchors.fill: parent
                visible: key.currentlyPressed || key.highlight
                source: key.imgPressed
            }

            /// label of the key
            //  the label is also the value subitted to the app

            Column {
                spacing: units.gu( UI.annotationMargins )
                anchors.centerIn: parent

                Text {
                    id: keyLabel
                    text: label
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: UI.fontFamily
                    font.pixelSize: fontSize
                    font.bold: UI.fontBold
                    color: UI.fontColor
                    textFormat: Text.StyledText
                }

                Text {
                    id: annotationLabel
                    text: annotation

                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottomMargin: units.gu( UI.annotationMargins )

                    font.pixelSize: units.gu( UI.annotationFontSize )
                    font.bold: false
                    color: UI.annotationFontColor
                    visible: annotation != ""
                }
            }
        }

        FlickPop {
            anchors.horizontalCenter: buttonImage.horizontalCenter
            anchors.bottom: buttonImage.top
            anchors.bottomMargin: key.height * 0.5
            width: units.gu((UI.fontSize + UI.flickMargin) * 3)
            height: units.gu((UI.fontSize + UI.flickMargin) * 3)
            chars: leaves
            index: keyFlickArea.index
            visible: key.currentlyPressed && chars.length > 1
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

            if (maliit_input_method.useAudioFeedback)
                audioFeedback.play();

            if (maliit_input_method.useHapticFeedback)
                 pressEffect.start();

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
