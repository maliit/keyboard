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
import "key_constants.js" as UI
import keys 1.0

ActionKey {
    label: action == "left" ? "⬅" : "➡";
    leftSide: action == "left";
    rightSide: action == "right";

    property string preedit: maliit_input_method.preedit
    property int cursorPosition: maliit_input_method.cursorPosition
    property bool isPreedit: preedit != ""

    overridePressArea: true;

    onReleased: {
        if (isPreedit) {
            if (action == "left" && cursorPosition > 0) {
                maliit_input_method.cursorPosition--
            } else if (action == "right" && cursorPosition < preedit.length) {
                maliit_input_method.cursorPosition++
            }
        } else {
            event_handler.onKeyReleased("", action);
        }
    }

    onPressed: {
        if (maliit_input_method.useAudioFeedback)
            audioFeedback.play();

        if (maliit_input_method.useHapticFeedback)
            pressEffect.start();

        if (!isPreedit)
            event_handler.onKeyPressed("", action);
    }

    onPressAndHold: {
        return;
    }
}
