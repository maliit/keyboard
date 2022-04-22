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
import QtQuick.Layouts 1.12

import MaliitKeyboard 2.0

Rectangle {

    id: wordRibbonCanvas
    objectName: "wordRibbenCanvas"
    state: "NORMAL"

    ListView {
        id: listView
        objectName: "wordListView"
        anchors.fill: parent

        model: WordModel

        orientation: ListView.Horizontal
        delegate: wordCandidateDelegate

        leftMargin: Device.gu(1)
        rightMargin: Device.gu(1)
        topMargin: 0
        bottomMargin: Device.top_margin
        spacing: Device.gu(2)
    }

    Component {
        id: wordCandidateDelegate
        RowLayout {
            id: wordCandidateItem
            // Use 1/3 of pixel height of parent converted to grid units
            // as a minimum width threshhold, so that short suggestions
            // are wide enough to tap with a thumb
            width: Math.max(Device.gu(height / 3), implicitWidth)
            height: wordRibbonCanvas.height
            spacing: listView.spacing

            Label {
                id: wordItem

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.fillHeight: true

                property bool textBold: isPrimaryCandidate || listView.count == 1 // Exposed for autopilot

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.pixelSize: parent.height - Device.top_margin * 2
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

            // FIXME: ListView doesn't support separators directly so we need
            // to be a little hacky to add a muted separator between word
            // candidates, so that multi-word suggestions are distinct
            Rectangle {
                Layout.alignment: Qt.AlignVCenter
                width: 1
                height: parent.height * 0.5
                color: wordItem.color
                opacity: 0.3
                visible: index != listView.count - 1
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
