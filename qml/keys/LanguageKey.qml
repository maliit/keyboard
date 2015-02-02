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

ActionKey {
    iconNormal: "language-chooser";
    iconShifted: "language-chooser";
    iconCapsLock: "language-chooser";

    padding: 0

    width: panel.keyWidth
    overridePressArea: true

    onPressed: {
        if (maliit_input_method.useAudioFeedback)
            audioFeedback.play();

        if (maliit_input_method.useHapticFeedback)
            pressEffect.start();

        if (panel.switchBack && panel.previousLanguage) {
            panel.switchBack = false;
            maliit_input_method.activeLanguage = panel.previousLanguage
        } else {
            panel.previousLanguage = maliit_input_method.activeLanguage
            canvas.languageMenuShown = true
        }
    }   
}
