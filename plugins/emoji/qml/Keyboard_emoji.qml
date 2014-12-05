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
import "../../keys"
import "../../keys/key_constants.js" as UI
import "emoji.js" as Emoji

KeyPad {
    anchors.fill: parent

    content: c1
    symbols: "languages/Keyboard_symbols.qml"
    property int offset: 740
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
                    label: chars[offset + index]
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
                    if (offset == 0) {
                        // Wrap around
                        offset = chars.length - 18
                    } else if (offset - 18 < 0) {
                        offset = 0
                    } else {
                        offset -= 18;
                    }
                }
            }

            Repeater {
                model: 8
                CharKey {
                    label: chars[10 + offset + index]
                    shifted: label
                }
            }

            ActionKey {
                iconNormal: "go-next"
                iconShifted: "go-next"
                iconCapsLock: "go-next"
                overridePressArea: true
                onPressed: {
                    if (offset + 18 == chars.length) {
                        // Wrap around
                        offset = 0
                    } else if (offset + 36 >= chars.length) {
                        offset = chars.length - 18
                    } else {
                        offset += 18
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
                onPressed: {
                    offset = 740
                }
            }

            ActionKey {
                label: "🚀"
                shifted: label
                overridePressArea: true
                onPressed: {
                    offset = 865
                }
            }

            ActionKey {
                label: "🕜"
                shifted: label
                overridePressArea: true
                onPressed: {
                    offset = 569
                }
            }

            ActionKey {
                label: "🐀"
                shifted: label
                overridePressArea: true
                onPressed: {
                    offset = 237
                }
            }

            ActionKey {
                label: "🏠"
                shifted: label
                overridePressArea: true
                onPressed: {
                    offset = 214
                }
            }

            ActionKey {
                label: "🌍"
                shifted: label
                overridePressArea: true
                onPressed: {
                    offset = 14
                }     
            }

            BackspaceKey { padding: 0; width: enterKey.width }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: panel.keyHeight + units.gu(UI.row_margin);

            SymbolShiftKey { id: symShiftKey;                            anchors.left: parent.left; height: parent.height; }
            LanguageKey    { id: languageMenuButton;                     anchors.left: symShiftKey.right; height: parent.height; switchBack: true }
            SpaceKey       { id: spaceKey;                               anchors.left: languageMenuButton.right; anchors.right: enterKey.left; noMagnifier: true; height: parent.height; }
            ReturnKey      { id: enterKey;                               anchors.right: parent.right; height: parent.height; }
        }
    } // column
}
