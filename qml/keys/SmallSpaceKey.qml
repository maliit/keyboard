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

import "languages.js" as Languages

import MaliitKeyboard 2.0

ActionKey {
    iconNormal: "keyboard-spacebar-symbolic";
    iconShifted: "keyboard-spacebar-symbolic";
    iconCapsLock: "keyboard-spacebar-symbolic";
    
    action: "space"
    switchBackFromSymbols: true
}
