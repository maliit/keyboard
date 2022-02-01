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
            anchors.margins: 0
            property alias word_text: wordItem // For testing in Autopilot
            property bool textBold: isPrimaryCandidate || listView.count == 1 // Exposed for autopilot

            Item {
                anchors.fill: parent
                anchors.margins: {
                    top: 0
                    bottom: 0
                    left: Device.gu(2)
                    right: Device.gu(2)
                }

                Label {
                    id: wordItem
                    font.pixelSize: Device.wordRibbonFontSize
                    font.family: Theme.fontFamily
                    font.weight: textBold ? Font.Bold : Font.Light
                    text: word;
                    anchors.centerIn: parent
                    color: Theme.fontColor
                }
            }

            MouseArea {
                anchors.fill: wordCandidateItem
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
