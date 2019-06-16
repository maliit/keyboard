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
import "languages.js" as Languages

ActionKey {
    label: " ";
    shifted: " ";

    normalColor: fullScreenItem.theme.charKeyColor
    pressedColor: fullScreenItem.theme.charKeyPressedColor

    action: "space"
    switchBackFromSymbols: true

    overridePressArea: true

    Label {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -parent.rowMargin / 2 - units.gu(0.15)
        font.family: UI.fontFamily
        font.weight: Font.Light
        font.pixelSize: parent.fontSize * 0.6
        opacity: UI.spaceOpacity
        text: Languages.languageIdToName(maliit_input_method.activeLanguage)
        horizontalAlignment: Text.AlignHCenter
        visible: !panel.hideKeyLabels
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
            fullScreenItem.keyFeedback();
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
