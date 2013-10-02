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
    property bool skipAutoCaps: false

    /* design */
    property string imgNormal: UI.imageCharKey
    property string imgPressed: UI.imageCharKeyPressed
    // fontSize can be overwritten when using the component, e.g. SymbolShiftKey uses smaller fontSize
    property int fontSize: units.gu( UI.fontSize );

    /// annotation shows a small label in the upper right corner
    // if the annotiation property is set, it will be used. If not, the first position in extended[] list or extendedShifted[] list will
    // be used, depending on the state. If no extended/extendedShifted arrays exist, no annotation is shown
    property string annotation: ""

    /* internal */
    property string __annotationLabelNormal
    property string __annotationLabelShifted

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
    property var activeExtendedModel: (panel.activeKeypad.state === "NORMAL") ? extended : extendedShifted

    Component.onCompleted: {
        if (annotation) {
            __annotationLabelNormal = annotation
            __annotationLabelShifted = annotation
        } else {
            if (extended)
                __annotationLabelNormal = extended[0]
            if (extendedShifted)
                __annotationLabelShifted = extendedShifted[0]
        }
    }

    BorderImage {
        id: buttonImage
        anchors.centerIn: parent
        anchors.fill: key
        anchors.margins: units.dp( UI.keyMargins );
    }

    /// label of the key
    //  the label is also the value subitted to the app

    Text {
        id: keyLabel
        text: (panel.activeKeypad.state === "NORMAL") ? label : shifted;
        anchors.centerIn: parent
        font.family: UI.fontFamily
        font.pixelSize: fontSize
        font.bold: UI.fontBold
        color: UI.fontColor
    }

    /// shows an annotation
    // used e.g. for indicating the existence of extended keys

    Text {
        id: annotationLabel
        text: (panel.activeKeypad.state != "NORMAL") ? __annotationLabelShifted : __annotationLabelNormal

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: units.gu( UI.annotationMargins )

        font.pixelSize: units.gu( UI.annotationFontSize )
        font.bold: false
        color: UI.annotationFontColor
    }

    PressArea {
        id: keyMouseArea
        anchors.fill: key

        onPressAndHold: {
            if (activeExtendedModel != undefined)
                PopupUtils.open(popoverComponent, keyMouseArea)
        }

        onReleased: {
            key.state = "NORMAL"

            if (!popoverHasFocus) {
                event_handler.onKeyReleased(valueToSubmit, action);
                if (!skipAutoCaps)
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
        width: key.width + units.gu(UI.magnifierHorizontalPadding)
        height: key.height + units.gu(UI.magnifierVerticalPadding)
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
