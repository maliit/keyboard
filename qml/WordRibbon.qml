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
import "keys/key_constants.js" as UI

Rectangle {

    id: wordRibbonCanvas
    objectName: "wordRibbenCanvas"
    state: "NORMAL"

    Rectangle {
        anchors.fill: parent
        color: UI.backgroundColor
    }

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
            width: wordItem.width + units.gu(2)
            height: wordRibbonCanvas.height
            anchors.margins: 0
            property alias word_text: wordItem // For testing in Autopilot
            property alias textBold: wordItem.font.bold // Exposed for autopilot

            Item {
                anchors.fill: parent
                anchors.margins: {
                    top: units.gu(0)
                    bottom: units.gu(0)
                    left: units.gu(2)
                    right: units.gu(2)
                }

                Label {
                    id: wordItem
                    fontSize: "large"
                    font.family: isPrimaryCandidate || listView.count == 1 ? "Ubuntu Light" : "Ubuntu Regular"
                    color: "#333333"
                    text: word;
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: units.gu(UI.top_margin) / 2.0
                }
            }

            MouseArea {
                anchors.fill: wordCandidateItem
                onPressed: {
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
                color: "transparent"
            }
        },
        State {
            name: "SELECTED"
            PropertyChanges {
                target: wordRibbonCanvas
                color: "#e4e4e4"
            }
        }
    ]

}

