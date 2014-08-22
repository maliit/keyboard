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
import Ubuntu.Components 0.1

ActionKey {
    iconNormal: "keyboard-caps"
    iconShifted: "keyboard-caps-active"
    iconCapsLock: "keyboard-caps-lock"

    // XXX: Disabled due to problems with coloring - see LP: #1251285 for details
    // colorShifted: UbuntuColors.orange

    action: "shift"

    MouseArea {
        anchors.fill: parent
        preventStealing: true

        onClicked: {
            if (maliit_input_method.useAudioFeedback)
                audioFeedback.play();

            if (maliit_input_method.useHapticFeedback)
                 pressEffect.start();

            if (panel.activeKeypadState == "NORMAL")
                panel.activeKeypadState = "SHIFTED";

            else if (panel.activeKeypadState == "SHIFTED")
                panel.activeKeypadState = "NORMAL"

            else if (panel.activeKeypadState == "CAPSLOCK")
                panel.activeKeypadState = "NORMAL"
        }

        onPressAndHold: {
            panel.activeKeypadState = "CAPSLOCK"

            if (maliit_input_method.useHapticFeedback)
                    pressEffect.start();
        }

        onDoubleClicked: {
            if (maliit_input_method.useAudioFeedback)
                audioFeedback.play();

            if (maliit_input_method.useHapticFeedback)
                 pressEffect.start();

            if (panel.activeKeypadState == "SHIFTED")
                panel.activeKeypadState = "CAPSLOCK"
        }
    }
}
