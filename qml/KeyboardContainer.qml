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
import QtQuick.Window 2.0
import "languages/"

Item {
    id: panel

    width: 720
    height: 486

    property int keyWidth: 0
    property int keyHeight: 0

    property var activeKeypad: characterKeypad.item

    state: "CHARACTERS"

    function loadLayout(layoutId)
    {
        if (layoutId === "number")
            characterKeypad.source = "languages/Keyboard_numbers.qml"
        if (layoutId === "phonenumber")
            characterKeypad.source = "languages/Keyboard_telephone.qml"
        if (layoutId === "email")
            characterKeypad.source = "languages/Keyboard_en_email.qml"
        if (layoutId === "url")
            characterKeypad.source = "languages/Keyboard_en_url_search.qml"
        if (layoutId === "en_us")
            characterKeypad.source = "languages/Keyboard_en_us.qml"
    }

    Loader {
        id: characterKeypad
        anchors.fill: parent

        source: "languages/Keyboard_en_us.qml"
    }

    Keyboard_symbols {
        id: symbolKeypad
    }

    states: [
        State {
            name: "CHARACTERS"
            PropertyChanges {
                target: panel
                activeKeypad: characterKeypad.item
            }
            PropertyChanges {
                target: characterKeypad
                visible: true
            }
            PropertyChanges {
                target: symbolKeypad
                visible: false
            }
        },
        State {
            name: "SYMBOLS"
            PropertyChanges {
                target: panel
                activeKeypad: symbolKeypad
            }
            PropertyChanges {
                target: characterKeypad
                visible: false
            }
            PropertyChanges {
                target: symbolKeypad
                visible: true
            }
        }
    ]

}

