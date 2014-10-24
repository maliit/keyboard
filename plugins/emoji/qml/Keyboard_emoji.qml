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

KeyPad {
    anchors.fill: parent

    content: c1
    symbols: "languages/Keyboard_symbols.qml"
    property string startChar: "😁"
    property int offset: 0

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            Repeater {
                model: 10
                CharKey {
                    label: String.fromCharCode(startChar.charCodeAt(0), startChar.charCodeAt(1) + index + offset);
                    shifted: label
                    leftSide: index == 0;
                    rightSide: index == 9;
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ActionKey {
                iconNormal: "go-previous"
                iconShifted: "go-previous"
                iconCapsLock: "go-previous"
                overridePressArea: true
                onPressed: {
                    offset -= 18
                }
            }

            Repeater {
                model: 8
                CharKey {
                    label: String.fromCharCode(startChar.charCodeAt(0), startChar.charCodeAt(1) + 10 + index + offset);
                    shifted: label
                }
            }

            ActionKey {
                iconNormal: "go-next"
                iconShifted: "go-next"
                iconCapsLock: "go-next"
                overridePressArea: true
                onPressed: {
                    offset += 18
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            ActionKey {
                label: "😁"
                shifted: label
                overridePressArea: true
                onPressed: {
                    startChar = label
                    offset = 0
                }
            }

            ActionKey {
                label: "🚀"
                shifted: label
                overridePressArea: true
                onPressed: {
                    startChar = label
                    offset = 0
                }
            }

            ActionKey {
                label: "🕜"
                shifted: label
                overridePressArea: true
                onPressed: {
                    startChar = label
                    offset = 0
                }
            }

            ActionKey {
                label: "🐀"
                shifted: label
                overridePressArea: true
                onPressed: {
                    startChar = label
                    offset = 0
                }
            }

            ActionKey {
                label: "🏠"
                shifted: label
                overridePressArea: true
                onPressed: {
                    startChar = label
                    offset = 0
                }
            }

            ActionKey {
                label: "🌍"
                shifted: label
                overridePressArea: true
                onPressed: {
                    startChar = label
                    offset = 0
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
