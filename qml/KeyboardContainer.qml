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
import "keys/"

Item {
    id: panel

    property int keyWidth: 0
    property int keyHeight: 0

    property variant activeKeypad: characterKeypadLoader.item
    property string activeKeypadState: characterKeypadLoader.item ? item.state : ""
    property string symbolKeypadSource

    state: "CHARACTERS"

    function loadLayout(layoutId)
    {
        if (layoutId === "number")
            characterKeypadLoader.setSource("languages/Keyboard_numbers.qml");
        if (layoutId === "phonenumber")
            characterKeypadLoader.setSource("languages/Keyboard_telephone.qml");
        if (layoutId === "email")
            characterKeypadLoader.setSource("languages/Keyboard_en_email.qml");
        if (layoutId === "url")
            characterKeypadLoader.setSource("languages/Keyboard_en_url_search.qml");
        if (layoutId === "en_us")
            characterKeypadLoader.setSource("languages/Keyboard_en_us.qml");
        if (layoutId === "zh_cn_pinyin")
            characterKeypadLoader.setSource("languages/Keyboard_zh_cn_pinyin.qml");
    }

    Loader {
        id: characterKeypadLoader
        anchors.fill: parent
        asynchronous: true
        source: "languages/Keyboard_en_us.qml"
        onLoaded: {
            symbolKeypadLoader.setSource(characterKeypadLoader.item.symbols)
            panel.state = "CHARACTERS"
        }
    }

    Loader {
        id: symbolKeypadLoader
        anchors.fill: parent
        asynchronous: true
    }

    ExtendedKeysSelector {
        id: extendedKeysSelector
        anchors.fill: parent
    }

    function closeExtendedKeys()
    {
        extendedKeysSelector.closePopover();
    }

    states: [
        State {
            name: "CHARACTERS"
            PropertyChanges {
                target: panel
                activeKeypad: characterKeypadLoader.item
            }
            PropertyChanges {
                target: characterKeypadLoader
                enabled: true
                opacity: 1
            }
            PropertyChanges {
                target: symbolKeypadLoader
                enabled: false
                opacity: 0
            }
        },
        State {
            name: "SYMBOLS"
            PropertyChanges {
                target: panel
                activeKeypad: symbolKeypadLoader.item
            }
            PropertyChanges {
                target: characterKeypadLoader
                enabled: false
                opacity: 0
            }
            PropertyChanges {
                target: symbolKeypadLoader
                enabled: true
                opacity: 1
            }
        }
    ]

}

