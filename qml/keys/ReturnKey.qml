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
    property string overrideIconName: input_method.actionKeyOverride ? input_method.actionKeyOverride.icon : ""
    property string overrideLabel: input_method.actionKeyOverride ? input_method.actionKeyOverride.icon : ""
    property string iconName: overrideIconName == "" &&  overrideLabel == "" ? "keyboard-return" : overrideIconName

    enabled: input_method.actionKeyOverride ? input_method.actionKeyOverride.enabled : true

    // overrideIcon has high priority over lable
    label: overrideIconName == "" ? overrideLabel : ""
    iconNormal: iconName
    iconShifted: iconName
    iconCapsLock: iconName
    action: "return"
    // TODO: input_method.actionKeyOverride.highlighted
}
