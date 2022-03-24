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

Rectangle {

    id: wordRibbonCanvas
    objectName: "wordRibbenCanvas"
    state: "NORMAL"

    ListView {
        id: listView
        objectName: "wordListView"
        anchors.fill: parent

        model: maliit_wordribbon

        orientation: ListView.Horizontal
        delegate: wordCandidateDelegate

    }

    Component {
        id: wordCandidateDelegate
        Item {
            id: wordCandidateItem
            width: wordItem.width + Device.gu(2)
            height: wordRibbonCanvas.height
            anchors.leftMargin: Device.gu(2)
            anchors.rightMargin: Device.gu(2)

            Label {
                id: wordItem

                property bool textBold: isPrimaryCandidate || listView.count == 1 // Exposed for autopilot

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: parent.height * 0.7
                font.family: Theme.fontFamily
                font.weight: textBold ? Font.Bold : Font.Light
                text: word
                color: Theme.fontColor

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        Feedback.keyPressed();

                        wordRibbonCanvas.state = "SELECTED"
                        event_handler.onWordCandidatePressed(wordItem.text, isUserInput)
                    }
                    onReleased: {
                        wordRibbonCanvas.state = "NORMAL"
                        event_handler.onWordCandidateReleased(wordItem.text, isUserInput)
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: wordRibbonCanvas
                color: Theme.backgroundColor
            }
        },
        State {
            name: "SELECTED"
            PropertyChanges {
                target: wordRibbonCanvas
                color: Theme.actionKeyPressedColor
            }
        }
    ]

}
