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
import QtQuick.Controls 2.12

import MaliitKeyboard 2.0

FlickCharKey {
    padding: Device.actionKeyPadding
    label: kana.label
    leaves: ["↻", "", "あ", "A"]

    overridePressArea: true

    property bool isPreedit: Keyboard.preedit.length > 0
    property string default_state: "kana"

    state: kana.state

    // Invisible text field to get selection color from style
    TextField {
        id: textArea
        width: 0
        height: 0
        visible: false
    }

    Item {
        id: kana
        property string label: "あ/A"

        state: parent.default_state;
        states: [
            State {
                name: "kana"
                PropertyChanges {
                    target: kana;
                    label: "<font color=\"" + textArea.selectionColor + "\">あ</font>/A";
                    state: "kana";
                }
            },
            State {
                name: "alnum"
                PropertyChanges {
                    target: kana;
                    label: "あ/<font color=\"" + textArea.selectionColor + "\">A</font>";
                    state: "alnum";
                }
            }
        ]
    }

    onReleased: {
        if (isPreedit) {
            event_handler.onKeyReleased("", "commit");
        }
        if (index == 0) {
            kana.state = kana.state == "kana" ? "alnum" : "kana"
        } else {
            if (index == 2) {
                kana.state = "kana"
            } else if (index == 3) {
                kana.state = "alnum"
            }
        }
    }

    onPressed: {
        Feedback.keyPressed();
    }
}
