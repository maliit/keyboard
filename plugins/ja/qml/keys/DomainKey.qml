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
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3

import "key_constants.js" as UI

FlickCharKey {
    property var unstyledLeaves: ["", "", "", "", ""];

    overridePressArea: true

    onReleased: {
        event_handler.onKeyReleased(unstyledLeaves[index], action);
    }

    onPressed: {
        if (maliit_input_method.useAudioFeedback)
            audioFeedback.play();

        if (maliit_input_method.useHapticFeedback)
            pressEffect.start();

        event_handler.onKeyPressed(unstyledLeaves[index], action);
    }
}
