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

    property Item activeKeypad: characterKeypadLoader.item
    property string activeKeypadState: characterKeypadLoader.item ? item.state : ""
    property string characterKeypadSource: ""
    property string symbolKeypadSource: ""


    state: "CHARACTERS"

    function loadLayout(layoutId)
    {
        if (layoutId === "number")
            characterKeypadSource = "languages/Keyboard_numbers.qml";
        if (layoutId === "phonenumber")
            characterKeypadSource = "languages/Keyboard_telephone.qml";
        if (layoutId === "email")
            characterKeypadSource = "languages/Keyboard_en_email.qml";
        if (layoutId === "url")
            characterKeypadSource = "languages/Keyboard_en_url_search.qml";
        if (layoutId === "en_us")
            characterKeypadSource = "languages/Keyboard_en_us.qml";
        if (layoutId === "zh_cn_pinyin")
            characterKeypadSource = "languages/Keyboard_zh_cn_pinyin.qml";
    }

    Loader {
        id: characterKeypadLoader
        objectName: "characterKeyPadLoader"
        anchors.fill: parent
        asynchronous: false
        enabled: panel.state === "CHARACTERS" ? true : false
        opacity: panel.state === "CHARACTERS" ? 1 : 0
        source: characterKeypadSource
        onLoaded: {
            symbolKeypadSource = activeKeypad.symbols
            panel.state = "CHARACTERS"
        }
    }

    Loader {
        id: symbolKeypadLoader
        objectName: "symbolKeyPadLoader"
        anchors.fill: parent
        enabled: panel.state === "SYMBOLS" ? true : false
        opacity: panel.state == "SYMBOLS" ? 1 : 0
        source: symbolKeypadSource
        asynchronous: false
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
        },
        State {
            name: "SYMBOLS"
            PropertyChanges {
                target: panel
                activeKeypad: symbolKeypadLoader.item
            }
        }
    ]

}

