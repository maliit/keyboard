/*
 * Copyright 2014 Canonical Ltd.
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

import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem
import Ubuntu.Components.Popups 1.3

import QtQuick.Window 2.0

import "key_constants.js" as UI

Item {
    id: popover

    property Item currentlyAssignedKey
    property alias anchorItem: __anchorItem

    property int currentlyAssignedKeyParentY: currentlyAssignedKey != null ? currentlyAssignedKey.parent.y : 0
    property int currentlyAssignedKeyX: currentlyAssignedKey != null ? currentlyAssignedKey.x : 0
    property int currentlyAssignedKeyY: currentlyAssignedKey != null ? currentlyAssignedKey.y : 0

    onCurrentlyAssignedKeyXChanged: __repositionPopoverTo(currentlyAssignedKey)
    onCurrentlyAssignedKeyYChanged: __repositionPopoverTo(currentlyAssignedKey)
    onCurrentlyAssignedKeyParentYChanged: __repositionPopoverTo(currentlyAssignedKey);

    onCurrentlyAssignedKeyChanged:
    {
        if (currentlyAssignedKey == null)
            return;

        __repositionPopoverTo(currentlyAssignedKey);
    }

    ///
    // Item gets repositioned above the currently active key on keyboard.
    // extended keys area will center on top of this
    Item {
        id: __anchorItem
        width: panel.keyWidth
        height: panel.keyHeight
    }

    function __repositionPopoverTo(item)
    {
        if(item) {
            var point = popover.mapFromItem(item, item.x, item.y)
            if (item.parent.parent.parent.objectName == "emojiGrid") {
                // The emoji layout uses a GridView, which requires
                // a different location calculation
                __anchorItem.x = point.x;
            } else { 
                __anchorItem.x = item.x + item.parent.x
            }
            __anchorItem.y = point.y - panel.keyHeight;
        }
    }
}
