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
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            ActionKey {
                iconNormal: "go-previous"
                iconShifted: "go-previous"
                iconCapsLock: "go-previous"
                overridePressArea: true
                width: panel.keyWidth * 1.5
                onPressed: {
                    if (internal.offset == 0) {
                        // Wrap around
                        internal.offset = internal.chars.length - 33
                    } else if (internal.offset - 33 < 0) {
                        internal.offset = 0
                    } else {
                        internal.offset -= 33;
                    }
                }
            }

            Repeater {
                model: 9
                CharKey {
                    label: internal.chars[24 + internal.offset + index]
                    shifted: label
                }
            }

            ActionKey {
                iconNormal: "go-next"
                iconShifted: "go-next"
                iconCapsLock: "go-next"
                overridePressArea: true
                width: panel.keyWidth * 1.5
                onPressed: {
                    if (internal.offset + 33 == internal.chars.length) {
                        // Wrap around
                        internal.offset = 0
                    } else if (internal.offset + 65 >= internal.chars.length) {
                        internal.offset = internal.chars.length - 33
                    } else {
                        internal.offset += 33
                    }
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            ActionKey {
                label: "😀"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 0 && internal.offset < 540
                onPressed: {
                    internal.offset = 0
                }
            }

            ActionKey {
                label: "🐶"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 540 && internal.offset < 701
                onPressed: {
                    internal.offset = 540
                }
            }

            ActionKey {
                label: "🍏"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 701 && internal.offset < 786
                onPressed: {
                    internal.offset = 701
                }
            }

            ActionKey {
                label: "⚽"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 786 && internal.offset < 1050
                onPressed: {
                    internal.offset = 786
                }
            }

            ActionKey {
                label: "💡"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 1050 && internal.offset < 1230
                onPressed: {
                    internal.offset = 1050
                }
            }

            ActionKey {
                label: "❤"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 1230 && internal.offset < 1514
                onPressed: {
                    internal.offset = 1230
                }
            }

            ActionKey {
                label: "🌍"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 1514
                onPressed: {
                    internal.offset = 1514
                }
            }

            BackspaceKey { padding: 0; width: enterKey.width }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; }
            SpaceKey       { id: spaceKey;                               anchors.left: languageMenuButton.right; anchors.right: enterKey.left; noMagnifier: true; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
