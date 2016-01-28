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

ActionKey {
    property var actionKeyOverride: maliit_input_method.actionKeyOverride
    property string overrideIconName: actionKeyOverride && actionKeyOverride.icon ? actionKeyOverride.icon : ""
    property string overrideLabel: actionKeyOverride && actionKeyOverride.label ? actionKeyOverride.label : ""

    enabled: actionKeyOverride ? actionKeyOverride.enabled : true
    // overrideIcon has high priority over label
    label: overrideIconName == "" ? overrideLabel : ""
    shifted: label

    iconNormal: (overrideIconName == "") && (overrideLabel == "") ? "keyboard-enter" : overrideIconName
    iconShifted: iconNormal
    iconCapsLock: iconNormal

    action: "return"
    switchBackFromSymbols: true
    // TODO: input_method.actionKeyOverride.highlighted
}
