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

import Ubuntu.Components 1.3

import "key_constants.js" as UI

CharKey {
    id: actionKeyRoot
    property string iconNormal: ""
    property string iconShifted: ""
    property string iconCapsLock: ""

    property string iconSourceNormal: ""
    property string iconSourceShifted: ""
    property string iconSourceCapsLock: ""

    noMagnifier: true
    skipAutoCaps: true
    property int padding: UI.actionKeyPadding

    // action keys are a bit wider
    width: panel.keyWidth + units.gu( padding )

    normalColor: fullScreenItem.theme.actionKeyColor
    pressedColor: fullScreenItem.theme.actionKeyPressedColor
    borderColor: fullScreenItem.theme.actionKeyBorderColor

    // can be overwritten by keys
    property color colorNormal: fullScreenItem.theme.fontColor
    property color colorShifted: fullScreenItem.theme.fontColor
    property color colorCapsLock: fullScreenItem.theme.fontColor

    // Make it possible for the visible area of the key to differ from the
    // actual key size. This allows us to extend the touch area of the bottom
    // row of keys all the way to the bottom of the keyboard, whilst
    // maintaining the same visual appearance.
    Item {
        anchors.top: parent.top
        height: panel.keyHeight
        width: parent.width

        Icon {
            id: iconImage
            source: iconSourceNormal !== "" ? iconSourceNormal
                                            : iconNormal ? "image://theme/%1".arg(iconNormal)
                                                         : ""
            color: actionKeyRoot.colorNormal
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -actionKeyRoot.rowMargin / 2 - units.gu(0.15)
            visible: (label == "" && !panel.hideKeyLabels)
            height: actionKeyRoot.fontSize
            width: height
        }
    }

    // make sure the icon changes even if the property icon* change on runtime
    state: panel.activeKeypadState
    states: [
        State {
            name: "SHIFTED"
            PropertyChanges {
                target: iconImage
                source: iconSourceShifted !== "" ? iconSourceShifted 
                                                 : iconShifted ? "image://theme/%1".arg(iconShifted)
                                                               : ""
                color: actionKeyRoot.colorShifted
            }
        },
        State {
            name: "CAPSLOCK"
            PropertyChanges {
                target: iconImage
                source: iconSourceCapsLock !== "" ? iconSourceCapsLock
                                                  : iconCapsLock ? "image://theme/%1".arg(iconCapsLock)
                                                                : ""
                color: actionKeyRoot.colorCapsLock
            }
        }
    ]
}
