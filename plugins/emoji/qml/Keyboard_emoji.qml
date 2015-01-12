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
import Ubuntu.Components 1.1
import "../../keys"
import "../../keys/key_constants.js" as UI
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
        property int offset: 536
        property var chars: calculateChars()

        function calculateChars() {
            var totalSkips = 0;
            var c = [];
            for (var block = 0; block < Emoji.start.length; block++) {
                for (var i = Emoji.start[block][1]; i < Emoji.end[block][1]; i++) {
                    while (Emoji.skip[block].indexOf(i) != -1) {
                        i++;
                    }
                    c.push(String.fromCharCode(Emoji.start[block][0], i));
                }
            }
            return c;
        }
    }

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0

            Repeater {
                model: 10
                CharKey {
                    label: internal.chars[internal.offset + index]
                    shifted: label
                    leftSide: index == 0
                    rightSide: index == 9
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
                onPressed: {
                    if (internal.offset == 0) {
                        // Wrap around
                        internal.offset = internal.chars.length - 18
                    } else if (internal.offset - 18 < 0) {
                        internal.offset = 0
                    } else {
                        internal.offset -= 18;
                    }
                }
            }

            Repeater {
                model: 8
                CharKey {
                    label: internal.chars[10 + internal.offset + index]
                    shifted: label
                }
            }

            ActionKey {
                iconNormal: "go-next"
                iconShifted: "go-next"
                iconCapsLock: "go-next"
                overridePressArea: true
                onPressed: {
                    if (internal.offset + 18 == internal.chars.length) {
                        // Wrap around
                        internal.offset = 0
                    } else if (internal.offset + 36 >= internal.chars.length) {
                        internal.offset = internal.chars.length - 18
                    } else {
                        internal.offset += 18
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
                highlight: internal.offset >= 536 && internal.offset < 613
                onPressed: {
                    internal.offset = 536
                }
            }

            ActionKey {
                label: "🚀"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 613
                onPressed: {
                    internal.offset = 613
                }
            }

            ActionKey {
                label: "🌍"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 0 && internal.offset < 173
                onPressed: {
                    internal.offset = 13
                }
            }

            ActionKey {
                label: "🏠"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 173 && internal.offset < 191
                onPressed: {
                    internal.offset = 173
                }
            }

            ActionKey {
                label: "🐀"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 191 && internal.offset < 508
                onPressed: {
                    internal.offset = 191
                }
            }

            ActionKey {
                label: "🕜"
                shifted: label
                overridePressArea: true
                highlight: internal.offset >= 508 && internal.offset < 536
                onPressed: {
                    internal.offset = 508
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
