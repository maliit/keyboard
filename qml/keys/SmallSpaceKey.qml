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

import QtQuick 2.4

import "key_constants.js" as UI
import "languages.js" as Languages

import MaliitKeyboard 2.0

ActionKey {
    normalColor: Theme.charKeyColor
    pressedColor: Theme.charKeyPressedColor

    iconNormal: "keyboard_spacebar";
    iconShifted: "keyboard_spacebar";
    iconCapsLock: "keyboard_spacebar";

    action: "space"
    switchBackFromSymbols: true
}
