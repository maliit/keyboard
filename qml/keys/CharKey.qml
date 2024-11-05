/*
 * Copyright 2013 Canonical Ltd.
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

    // to be set in keyboard layouts
    property string iconNormal: ""
    property string iconShifted: ""
    property string iconCapsLock: ""
    property string label: ""
    property string shifted: ""
    property var extended; // list of extended keys
    property var extendedShifted; // list of extended keys in shifted state
    property var currentExtendedKey; // The currently highlighted extended key
    property bool highlight: false;
    property double textCenterOffset: Device.gu(-0.15)

    property string valueToSubmit: keyLabel.text

    property alias acceptDoubleClick: keyMouseArea.acceptDoubleClick
    property alias horizontalSwipe: keyMouseArea.horizontalSwipe

    property string action
    property bool noMagnifier: false
    property bool skipAutoCaps: false
    property bool switchBackFromSymbols: false

    property bool leftSide: false
    property bool rightSide: false

    property double rowMargin: fullScreenItem.landscape ? Device.rowMarginLandscape
                                                        : Device.rowMarginPortrait
    property double keyMargin: Device.keyMargins

    // These properties are used by autopilot to determine the visible
    // portion of the key to press
    readonly property double leftOffset: keyButton.anchors.leftMargin
    readonly property double rightOffset: keyButton.anchors.rightMargin

    // Scale the font so the label fits if a long word is set
    property int fontSize: (fullScreenItem.landscape ? (height / 2) : (height / 2.8))
                           * (4 / (label.length >= 2 ? (label.length <= 6 ? label.length + 2.5 : 8) : 4));
    property alias fontFamily: keyLabel.font.family

    /// annotation shows a small label in the upper right corner
    // if the annotiation property is set, it will be used. If not, the first position in extended[] list or extendedShifted[] list will
    // be used, depending on the state. If no extended/extendedShifted arrays exist, no annotation is shown
    property string annotation: ""

    /*! indicates if the key is currently pressed/down*/
    property alias currentlyPressed: keyMouseArea.pressed

    /* internal */
    property string __annotationLabelNormal
    property string __annotationLabelShifted

    /**
     * this property specifies if the key can submit its value or not (e.g. when the popover is shown, it does not commit its value)
     */

    property bool extendedKeysShown: extendedKeysSelector.enabled

    /*
     * label changes when keyboard is in shifted mode
     * extended keys change as well when shifting keyboard, typically lower-uppercase: ê vs Ê
     */

    property string oskState: panel.activeKeypadState
    property var activeExtendedModel: (panel.activeKeypadState === "NORMAL") ? extended : extendedShifted

    // Allow action keys to override the standard key behaviour
    property bool overridePressArea: false

    // Allow to manipulate preedit if it need.
    // if allowPreeditHandler is enabled should be assigned preeditHandler.
    property bool allowPreeditHandler: false
    property var preeditHandler: null


    signal pressed()
    signal released()
    signal pressAndHold()
    signal doubleClicked()
    signal keySent(string key)

    Component.onCompleted: {
        if (annotation) {
            __annotationLabelNormal = annotation
            __annotationLabelShifted = annotation
        } else {
            if (extended && extended[0])
                __annotationLabelNormal = extended[0]
            if (extendedShifted && extendedShifted[0])
                __annotationLabelShifted = extendedShifted[0]
        }
    }

    // Make it possible for the visible area of the key to differ from the
    // actual key size. This allows us to extend the touch area of the bottom
    // row of keys all the way to the bottom of the keyboard, whilst
    // maintaining the same visual appearance.
    Item {
        anchors.top: parent.top
        height: panel.keyHeight
        width: parent.width

        ToolButton {
            id: keyButton
            anchors.fill: parent
            anchors.leftMargin: key.leftSide ? (parent.width - panel.keyWidth) + key.keyMargin : key.keyMargin
            anchors.rightMargin: key.rightSide ? (parent.width - panel.keyWidth) + key.keyMargin : key.keyMargin
            anchors.bottomMargin: key.rowMargin

            // Disable hover so that highlight doesn't stick on touch screens
            hoverEnabled: false

            // Tell the ToolButton to be drawn as pressed, when pressed
            down: keyMouseArea.pressed

            // Icon of the key
            icon.name: key.iconNormal
            icon.height: key.fontSize
            icon.width: key.fontSize

            display: label == "" ? AbstractButton.IconOnly : AbstractButton.TextOnly

            /// label of the key
            //  the label is also the value subitted to the app

            Label {
                id: keyLabel
                text: label
                font.pixelSize: key.fontSize
                font.weight: Font.Light
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.leftMargin: Device.gu(0.2)
                anchors.rightMargin: Device.gu(0.2)
                anchors.verticalCenter: parent.verticalCenter 
                anchors.verticalCenterOffset: key.textCenterOffset
                horizontalAlignment: Text.AlignHCenter
                // Avoid eliding characters that are slightly too wide (e.g. some emoji and chinese characters)
                elide: text.length <= 4 ? Text.ElideNone : Text.ElideRight
            }

            /// shows an annotation
            // used e.g. for indicating the existence of extended keys

            Label {
                id: annotationLabel
                text: __annotationLabelNormal

                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: Device.annotationTopMargin
                anchors.rightMargin: Device.annotationRightMargin
                font.pixelSize: key.fontSize / 2.25
                font.weight: Font.Light
            }

        }
    }

    // make sure the icon and/or labels change with the state
    state: panel.activeKeypadState
    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: keyButton
                icon.name: iconNormal
            }
            PropertyChanges {
                target: keyLabel
                text: label
            }
            PropertyChanges {
                target: annotationLabel
                text: __annotationLabelNormal
            }
        },
        State {
            name: "SHIFTED"
            PropertyChanges {
                target: keyButton
                icon.name: iconShifted ? iconShifted : iconNormal
            }
            PropertyChanges {
                target: keyLabel
                text: shifted
            }
            PropertyChanges {
                target: annotationLabel
                text: __annotationLabelShifted
            }
        },
        State {
            name: "CAPSLOCK"
            PropertyChanges {
                target: keyButton
                icon.name: iconCapsLock ? iconCapsLock : iconShifted ? iconShifted : iconNormal
            }
            PropertyChanges {
                target: keyLabel
                text: shifted
            }
        }
    ]

    PressArea {
        id: keyMouseArea
        anchors.fill: parent

        onPressAndHold: {
            if (overridePressArea) {
                key.pressAndHold();
                return;
            }
            if (activeExtendedModel != undefined) {
                Feedback.startPressEffect();

                magnifier.shown = false
                extendedKeysSelector.enabled = true
                extendedKeysSelector.extendedKeysModel = activeExtendedModel
                extendedKeysSelector.currentlyAssignedKey = key
                var extendedKeys = extendedKeysSelector.keys;
                var middleKey = extendedKeys.length > 1 ? Math.floor(extendedKeys.length / 2) - 1: 0;
                if (extendedKeys.length > 5 && extendedKeysSelector.multirow) {
                    middleKey = extendedKeys.length - middleKey - 1;
                }
                extendedKeys[middleKey].highlight = true;
                currentExtendedKey = extendedKeys[middleKey];
            }
        }

        onMouseXChanged: {
            evaluateSelectorSwipe();
        }

        onMouseYChanged: {
            evaluateSelectorSwipe();
        }

        onReleased: {
            key.released();
            if (overridePressArea) {
                return;
            }
            if (extendedKeysShown) {
                if (currentExtendedKey) {
                    currentExtendedKey.commit();
                    currentExtendedKey = null;
                } else {
                    extendedKeysSelector.closePopover();
                }
            } else if(!swipedOut) {
                // Read this prior to altering autocaps
                var keyToSend = valueToSubmit;
                if (magnifier.currentlyAssignedKey == key) {
                    magnifier.shown = false;
                }

                if (panel.autoCapsTriggered && action != "backspace") {
                    panel.autoCapsTriggered = false;
                }
                else if (!skipAutoCaps) {
                    if (panel.activeKeypadState === "SHIFTED" && panel.state === "CHARACTERS")
                        panel.activeKeypadState = "NORMAL";
                    if (panel.activeKeypadState === "CONTROL" && panel.state === "CHARACTERS")
                        panel.activeKeypadState = "NORMAL";
                }
                if (switchBackFromSymbols && panel.state === "SYMBOLS") {
                    panel.state = "CHARACTERS";
                }

                if (allowPreeditHandler && preeditHandler) {
                    preeditHandler.onKeyReleased(keyToSend, action);
                    return;
                }

                event_handler.onKeyReleased(keyToSend, action);
                keySent(keyToSend);
            } else if (action == "backspace") {
                // Send release from backspace if we're swiped out since
                // backspace activates on press and deactivates on release
                // to allow for repeated backspaces, unlike normal keys
                // which activate on release.
                event_handler.onKeyReleased(valueToSubmit, action);
                keySent(valueToSubmit);
            }
        }

        onSwipedOutChanged: {
            if(swipedOut && magnifier.currentlyAssignedKey == key) {
                magnifier.shown = false;
            }
        }

        onPressed: {
            key.pressed();
            if (overridePressArea) {
                return;
            }
            magnifier.currentlyAssignedKey = key
            magnifier.shown = !noMagnifier && Keyboard.enableMagnifier

            Feedback.keyPressed();

            // Quick workaround to fix initial autocaps - not beautiful, but works
            if(action != "backspace") {
                panel.autoCapsTriggered = false;
            }
            event_handler.onKeyPressed(valueToSubmit, action);
        }

        onDoubleClicked: {
            key.doubleClicked();
            if (overridePressArea) {
                return;
            }
        }

        function evaluateSelectorSwipe() {
            currentExtendedKey = extendedKeysSelector.evaluateSelectorSwipe(mouseX, mouseY);
        }
    }

    Connections {
        target: swipeArea.drag
        function onActiveChanged() {
            if (swipeArea.drag.active)
                keyMouseArea.cancelPress();
        }
    }
}
