/*
 * Copyright 2015 Canonical Ltd.
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

import MaliitKeyboard 2.0

import "modifier.js" as Modifier

FlickCharKey {
    label: (isPreedit && layoutState == "kana") ? " ゛ ゜" : "␣"
    leaves: isPreedit ? preeditLeaves : ["␣", "□"]
    annotation: isPreedit ? "大⇔小" : ""

    overridePressArea: true

    property string layoutState: "kana"
    property string preedit: Keyboard.preedit
    property int cursorPosition: Keyboard.cursorPosition
    property bool isPreedit: preedit != ""
    property string lastChar: ""
    property var preeditLeaves: Modifier.map[lastChar] ? Modifier.map[lastChar] : [lastChar]

    onReleased: {
        if (isPreedit) {
            if (layoutState == "alnum") {
                if (index != 0) {
                    event_handler.onKeyReleased("", "space");
                } else {
                    var pos = cursorPosition
                    var newChar = lastChar.charCodeAt(0) < 91 ? lastChar.toLowerCase() : lastChar.toUpperCase()
                    Keyboard.preedit = preedit.substr(0, cursorPosition-1) + newChar + preedit.substr(cursorPosition)
                    Keyboard.cursorPosition = pos
                }
            } else if (preeditLeaves[index] && preeditLeaves[index] != "") {
                var pos = cursorPosition
                Keyboard.preedit = preedit.substr(0, cursorPosition-1) + preeditLeaves[index] + preedit.substr(cursorPosition)
                Keyboard.cursorPosition = pos
            }
        } else {
            if (index == 0) {
                event_handler.onKeyReleased("", "space");
            } else {
                event_handler.onKeyReleased("　", "");
            }
        }
    }

    onPressed: {
        Feedback.keyPressed();

        if (isPreedit) {
            lastChar = preedit.charAt(cursorPosition-1)
            if (!Modifier.map[lastChar] && Modifier.normalize[lastChar]) {
                lastChar = Modifier.normalize[lastChar]
            }
        }
    }
}
