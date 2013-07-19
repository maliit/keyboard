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

Rectangle {

    id: wordRibbonCanvas
    state: "NORMAL"

    Rectangle {
        anchors.fill: parent
        color: "#f1f1f1"
    }

    ListView {
        id: listView
        anchors.fill: parent;

        model: maliit_wordribbon

        orientation: ListView.Horizontal
        delegate: wordCandidateDelegate

    }

    Component {
        id: wordCandidateDelegate
        Item {
            id: wordCandidateItem
            width: wordItem.width + units.gu(2);
            height: wordRibbonCanvas.height

            Item {
                anchors.fill: parent
                anchors.margins: units.gu(1);

                Text {
                    id: wordItem
                    font.pixelSize: units.gu(2);
                    font.family: "Ubuntu Medium"
                    color: "#999999"
                    font.bold: false
                    text: word;
                }
            }

            MouseArea {
                anchors.fill: wordCandidateItem
                onPressed: {
                    wordRibbonCanvas.state = "SELECTED"
                    event_handler.onWordCandidatePressed(wordItem.text);
                }
                onReleased: {
                    wordRibbonCanvas.state = "NORMAL"
                    event_handler.onWordCandidateReleased(wordItem.text)
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

