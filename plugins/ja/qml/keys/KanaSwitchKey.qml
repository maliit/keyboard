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
import QtMultimedia 5.0
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

import "key_constants.js" as UI

FlickCharKey {
    padding: UI.actionKeyPadding
    label: kana.label
    leaves: ["↻", "", "あ", "A"]
    normalColor: fullScreenItem.theme.actionKeyColor
    pressedColor: fullScreenItem.theme.actionKeyPressedColor
    borderColor: fullScreenItem.theme.actionKeyBorderColor

    overridePressArea: true

    property bool isPreedit: maliit_input_method.preedit.length > 0
    property string default_state: "kana"

    state: kana.state

    Item {
        id: kana
        property string label: "あ/A"

        state: parent.default_state;
        states: [
            State {
                name: "kana"
                PropertyChanges {
                    target: kana;
                    label: "<font color=\"" + fullScreenItem.theme.selectionColor + "\">あ</font>/A";
                    state: "kana";
                }
            },
            State {
                name: "alnum"
                PropertyChanges {
                    target: kana;
                    label: "あ/<font color=\"" + fullScreenItem.theme.selectionColor + "\">A</font>";
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
        if (maliit_input_method.useAudioFeedback)
            audioFeedback.play();

        if (maliit_input_method.useHapticFeedback)
            pressEffect.start();
    }
}
