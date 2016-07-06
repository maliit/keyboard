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

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Repeater {
                model: 12
                CharKey {
                    label: internal.chars[internal.offset + index]
                    shifted: label
                    leftSide: index == 0
                    rightSide: index == 11
                    normalColor: UI.backgroundColor
                    fontSize: fullScreenItem.landscape ? height / 1.8 : height / 2.5
                    horizontalSwipe: true
                    onSwipeLeft: c1.pageRight()
                    onSwipeRight: c1.pageLeft()
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Repeater {  
                model: 12
                CharKey {
                    label: internal.chars[11 + internal.offset + index]
                    shifted: label
                    leftSide: index == 0
                    rightSide: index == 11
                    normalColor: UI.backgroundColor
                    fontSize: fullScreenItem.landscape ? height / 1.8 : height / 2.5
                    horizontalSwipe: true
                    onSwipeLeft: c1.pageRight()
                    onSwipeRight: c1.pageLeft()
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Repeater {
                model: 12
                CharKey {
                    label: internal.chars[24 + internal.offset + index]
                    shifted: label
                    leftSide: index == 0
                    rightSide: index == 11
                    normalColor: UI.backgroundColor
                    fontSize: fullScreenItem.landscape ? height / 1.8 : height / 2.5
                    horizontalSwipe: true
                    onSwipeLeft: c1.pageRight()
                    onSwipeRight: c1.pageLeft()
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Repeater {
                model: 12
                CharKey {
                    label: internal.chars[36 + internal.offset + index]
                    shifted: label
                    leftSide: index == 0
                    rightSide: index == 11
                    normalColor: UI.backgroundColor
                    fontSize: fullScreenItem.landscape ? height / 1.8 : height / 2.5
                    horizontalSwipe: true
                    onSwipeLeft: c1.pageRight()
                    onSwipeRight: c1.pageLeft()
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            LanguageKey    { id: languageMenuButton; width: panel.keyWidth * 2 }

            CategoryKey {
                label: "🕒"
                highlight: false
                onPressed: {

                }
            }           
 
            CategoryKey {
                label: "😀"
                highlight: internal.offset >= 0 && internal.offset < 540
                onPressed: {
                    internal.offset = 0
                }
            }

            CategoryKey {
                label: "🐶"
                highlight: internal.offset >= 540 && internal.offset < 701
                onPressed: {
                    internal.offset = 540
                }
            }

            CategoryKey {
                label: "🍏"
                highlight: internal.offset >= 701 && internal.offset < 786
                onPressed: {
                    internal.offset = 701
                }
            }

            CategoryKey {
                label: "⚽"
                highlight: internal.offset >= 786 && internal.offset < 1050
                onPressed: {
                    internal.offset = 786
                }
            }

            CategoryKey {
                label: "💡"
                highlight: internal.offset >= 1050 && internal.offset < 1230
                onPressed: {
                    internal.offset = 1050
                }
            }

            CategoryKey {
                label: "❤"
                highlight: internal.offset >= 1230 && internal.offset < 1514
                onPressed: {
                    internal.offset = 1230
                }
            }

            CategoryKey {
                label: "🌍"
                highlight: internal.offset >= 1514
                onPressed: {
                    internal.offset = 1514
                }
            }

            BackspaceKey { padding: 0; width: panel.keyWidth * 2 }
        }

        function pageLeft() {
            if (internal.offset == 0) {
                // Wrap around
                internal.offset = internal.chars.length - 48
            } else if (internal.offset - 48 < 0) {
                internal.offset = 0
            } else {
                internal.offset -= 48;
            }
        }

        function pageRight() {
            if (internal.offset + 48 == internal.chars.length) {
                // Wrap around
                internal.offset = 0
            } else if (internal.offset + 96 >= internal.chars.length) {
                internal.offset = internal.chars.length - 48
            } else {
                internal.offset += 48
            }
        }

    } // column
}
