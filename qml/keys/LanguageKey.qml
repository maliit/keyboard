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
    property bool switchBack: false // Switch back to previous layout without showing menu

    iconNormal: "language-chooser";
    iconShifted: "language-chooser";
    iconCapsLock: "language-chooser";

    padding: 0

    visible: maliit_input_method.enabledLanguages.length > 1 ? true : false
    width: visible ? panel.keyWidth : 0
    overridePressArea: true

    onPressed: {
        if (maliit_input_method.useAudioFeedback)
            audioFeedback.play();

        if (maliit_input_method.useHapticFeedback)
            pressEffect.start();

        if (switchBack && panel.previousLanguage) {
            maliit_input_method.activeLanguage = panel.previousLanguage
        } else {
            panel.previousLanguage = maliit_input_method.activeLanguage
            canvas.languageMenuShown = true
        }
    }   
}
