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
import "keys/key_constants.js" as UI

Rectangle {

    id: wordRibbonCanvas
    objectName: "wordRibbenCanvas"
    state: "NORMAL"

    Rectangle {
        anchors.fill: parent
        color: fullScreenItem.theme.backgroundColor
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
            property bool textBold: isPrimaryCandidate || listView.count == 1 // Exposed for autopilot

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
                    font.pixelSize: units.dp(fullScreenItem.tablet ? UI.tabletWordRibbonFontSize : UI.phoneWordRibbonFontSize)
                    font.family: UI.fontFamily
                    font.weight: textBold ? Font.Bold : Font.Light
                    text: word;
                    anchors.centerIn: parent
                }
            }

            MouseArea {
                anchors.fill: wordCandidateItem
                onPressed: {
                    fullScreenItem.keyFeedback();
                    
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

