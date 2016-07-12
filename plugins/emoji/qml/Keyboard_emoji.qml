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
import keys 1.0
import "emoji.js" as Emoji

KeyPad {
    anchors.fill: parent

    content: c1
    symbols: "languages/Keyboard_symbols.qml"

    Component.onCompleted: {
        panel.switchBack = true;
    }

    QtObject {
        id: internal
        property int offset: 0
        property var chars: Emoji.emoji
    }

    GridView {
        id: c1
        objectName: "emojiGrid"
        property int lastVisibleIndex: indexAt(contentX + (width / 2), 0);
        property int numberOfRows: 5
        property int maxNrOfKeys: 10
        anchors.top: parent.top
        anchors.bottom: categories.top
        anchors.left: parent.left
        anchors.right: parent.right
        model: Emoji.emoji
        flow: GridView.FlowTopToBottom
        flickDeceleration: units.gu(500)
        snapMode: GridView.SnapToRow
        cellWidth: panel.keyWidth
        cellHeight: panel.keyHeight
        cacheBuffer: 1000
        onContentXChanged: {
            magnifier.shown = false;
            magnifier.currentlyAssignedKey = null;
        }
        delegate: CharKey {
            label: modelData
            shifted: label
            normalColor: UI.backgroundColor
            pressedColor: UI.backgroundColor
            fontSize: fullScreenItem.landscape ? height / 1.8 : height / 2.5
        }

     }

     Row {
            id: categories
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: panel.keyHeight

            spacing: 0

            LanguageKey {
                id: languageMenuButton
                label: "ABC"
                shifted: label
                normalColor: UI.backgroundColor
                pressedColor: UI.backgroundColor
            }

            CategoryKey {
                label: "🕒"
                highlight: false
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                }
            }           
 
            CategoryKey {
                label: "😀"
                highlight: c1.lastVisibleIndex < 540
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(0, GridView.Beginning)
                }
            }

            CategoryKey {
                label: "🐶"
                highlight: c1.lastVisibleIndex >= 540 && c1.lastVisibleIndex < 701
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(540, GridView.Beginning)
                }
            }

            CategoryKey {
                label: "🍏"
                highlight: c1.lastVisibleIndex >= 701 && c1.lastVisibleIndex < 786
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(701, GridView.Beginning)
                }
            }

            CategoryKey {
                label: "🎾"
                highlight: c1.lastVisibleIndex >= 786 && c1.lastVisibleIndex < 931
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(786, GridView.Beginning)
                }
            }

            CategoryKey {
                label: "🚗"
                highlight: c1.lastVisibleIndex >= 931 && c1.lastVisibleIndex < 1050
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(931, GridView.Beginning)
                }
            }

            CategoryKey {
                label: "💡"
                highlight: c1.lastVisibleIndex >= 1050 && c1.lastVisibleIndex < 1230
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(1050, GridView.Beginning)
                }
            }

            CategoryKey {
                label: "❤"
                highlight: c1.lastVisibleIndex >= 1230 && c1.lastVisibleIndex < 1514
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(1230, GridView.Beginning)
                }
            }

            CategoryKey {
                label: "🌍"
                highlight: c1.lastVisibleIndex >= 1514
                onPressed: {
                    if (maliit_input_method.useHapticFeedback)
                        pressEffect.start();
                    c1.positionViewAtIndex(1514, GridView.Beginning)
                }
            }

            BackspaceKey {
                padding: 0
                normalColor: UI.backgroundColor
                pressedColor: UI.backgroundColor
            }
        }
}
