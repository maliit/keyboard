/*
 * Copyright 2013 Canonical Ltd.
 * Copyright (C) 2020 UBports Foundation.
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

import keys 1.0

CharKey {
    // Some Thai vowel is put above or below character, thus requiring an
    // extra character for the label to show properly. Modifies the
    // valueToSubmit property to strip such extra character.

    readonly property string labelStripped: label.slice(-1)
    readonly property string shiftedStripped: shifted.slice(-1)

    valueToSubmit: (panel.activeKeypadState === "NORMAL")
                        ? labelStripped
                        : shiftedStripped;
}
