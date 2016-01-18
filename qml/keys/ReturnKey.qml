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
import "key_constants.js" as UI

ActionKey {
    property var actionKeyOverride: maliit_input_method.actionKeyOverride
    property string overrideIconName: actionKeyOverride && actionKeyOverride.icon ? actionKeyOverride.icon : ""
    property string overrideLabel: actionKeyOverride && actionKeyOverride.label ? actionKeyOverride.label : ""
    anchors.rightMargin: units.gu(UI.keyMargins)

    enabled: actionKeyOverride ? actionKeyOverride.enabled : true
    // overrideIcon has high priority over label
    label: overrideIconName == "" ? overrideLabel : ""
    // Scale the font so the label fits if a long word is set
    fontSize: (height / 3) * (4 / (label.length >= 4 ? (label.length <= 6 ? label.length : 6) : 4));
    shifted: label

    iconNormal: (overrideIconName == "") && (overrideLabel == "") ? "keyboard-return" : overrideIconName
    iconShifted: iconNormal
    iconCapsLock: iconNormal

    action: "return"
    switchBackFromSymbols: true
    // TODO: input_method.actionKeyOverride.highlighted
}
