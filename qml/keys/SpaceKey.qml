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

import QtQuick.Controls 2.1

import MaliitKeyboard 2.0

import "languages.js" as Languages

ActionKey {
    label: " ";
    shifted: " ";

    normalColor: Theme.charKeyColor
    pressedColor: Theme.charKeyPressedColor

    action: "space"
    switchBackFromSymbols: true

    overridePressArea: true

    Label {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.rowMargin / 2 - Device.gu(0.15)
        font.family: Theme.fontFamily
        font.weight: Font.Light
        font.pixelSize: parent.fontSize * 0.6
        opacity: Theme.spaceOpacity
        text: Languages.languageIdToName(Keyboard.activeLanguage)
        horizontalAlignment: Text.AlignHCenter
        visible: !panel.hideKeyLabels
        color: Theme.fontColor
    }

    MouseArea {
        id: swipeArea
        anchors.fill: parent

        onPressAndHold: {
            fullScreenItem.prevSwipePositionX = mouseX
            fullScreenItem.prevSwipePositionY = mouseY
            fullScreenItem.cursorSwipe = true
            spaceKey.currentlyPressed = false
        }

        onPressed: {
            Feedback.keyPressed()
            spaceKey.currentlyPressed = true
            fullScreenItem.timerSwipe.stop()
        }
        onReleased: {
            if (fullScreenItem.cursorSwipe) {
                fullScreenItem.timerSwipe.restart()
            } else {
                spaceKey.currentlyPressed = false
                event_handler.onKeyReleased("", "space")
                if (switchBackFromSymbols && panel.state === "SYMBOLS") {
                    panel.state = "CHARACTERS"
                }
            }
        }

        onMouseXChanged: {
            if (fullScreenItem.cursorSwipe) {
                fullScreenItem.processSwipe(mouseX, mouseY);
            }
        }
    }

}
