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

import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Popups 0.1

import "key_constants.js" as UI

Item {
    id: key

    property int padding: 0

    width: panel.keyWidth
    height: panel.keyHeight

    /* to be set in keyboard layouts */
    property string label: ""
    property string shifted: ""
    property var extended; // list of extended keys
    property var extendedShifted; // list of extended keys in shifted state

    property alias valueToSubmit: keyLabel.text

    property string action
    property bool noMagnifier: false

    /* design */
    property string imgNormal: UI.imageCharKey
    property string imgPressed: UI.imageCharKeyPressed
    // fontSize can be overwritten when using the component, e.g. SymbolShiftKey uses smaller fontSize
    property int fontSize: units.gu( UI.fontSize );

    state: "NORMAL"

    /**
     * this property specifies if the key can submit its value or not (e.g. when the popover is shown, it does not commit its value)
     */

    property bool popoverHasFocus: false

    /*
     * label changes when keyboard is in shifted mode
     * extended keys change as well when shifting keyboard, typically lower-uppercase: ê vs Ê
     */
    property string oskState: panel.activeKeypad.state
    property var activeExtendedModel: extended

    onOskStateChanged: {
        if (panel.activeKeypad.state == "NORMAL") {
            keyLabel.text = label;
            activeExtendedModel = extended;
        } else if (panel.activeKeypad.state == "SHIFTED") {
            keyLabel.text = shifted;
            activeExtendedModel = extendedShifted
        }
        // CAPSLOCK keeps everything as in SHIFTED, nothing to do
    }

    BorderImage {
        id: buttonImage
        border { left: 22; top: 22; right: 22; bottom: 22 }
        anchors.centerIn: parent
        anchors.fill: key
        anchors.margins: units.dp( UI.keyMargins );
    }

    Text {
        id: keyLabel
        text: ""
        anchors.centerIn: parent
        font.family: UI.fontFamily
        font.pixelSize: fontSize
        font.bold: UI.fontBold
        color: UI.fontColor
    }

    MouseArea {
        id: keyMouseArea
        anchors.fill: key

        hoverEnabled: true
        preventStealing: true

        onPressAndHold: {
            if (activeExtendedModel != undefined)
                PopupUtils.open(popoverComponent, keyMouseArea)
        }

        onReleased: {
            key.state = "NORMAL"

            if (!popoverHasFocus) {
                event_handler.onKeyReleased(valueToSubmit, action);

                if (panel.activeKeypad.state === "SHIFTED" && panel.state === "CHARACTERS")
                    panel.activeKeypad.state = "NORMAL"
            }
        }
        onPressed: {
            key.state = "PRESSED"
            event_handler.onKeyPressed(valueToSubmit);
        }
    }

    Component {
        id: popoverComponent
        ExtendedKeysSelector {
            id: extendedKeysSelector
            extendedKeysModel: activeExtendedModel
            onVisibleChanged: popoverHasFocus = visible;
        }
    }

    Popper {
        id: popper
        visible: !noMagnifier && !popoverHasFocus
        width: panel.keyWidth
        height: panel.keyHeight + units.gu(UI.magnifierVerticalPadding)
    }

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: buttonImage
                source: imgNormal
            }
            PropertyChanges {
                target: popper
                popTarget: null
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: buttonImage
                source: imgPressed
            }
            PropertyChanges {
                target: popper
                popTarget: keyLabel
            }
        }
    ]
}
