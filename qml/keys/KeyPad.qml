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

Item {
    id: keyPadRoot

    state: "NORMAL"

    property var content: c1
    property string symbols: "languages/Keyboard_symbols.qml"
    property bool capsLock: false

    Column {
        id: c1
    }

    Component.onCompleted:
    {
        calculateKeyWidth();
        calculateKeyHeight();
    }

    onWidthChanged: calculateKeyWidth()
    onHeightChanged: calculateKeyHeight();

    function numberOfRows() {
        return content.children.length;
    }

    // we don´t use a QML layout, because we want all keys to be equally sized
    function calculateKeyWidth() {
        var width = panel.width;

        var maxNrOfKeys = 0;
        for (var i = 0; i < numberOfRows(); ++i) {
            if (content.children[i].children.length > maxNrOfKeys)
                maxNrOfKeys = content.children[i].children.length;
        }

        var maxSpaceForKeys = panel.width / maxNrOfKeys;

        panel.keyWidth = maxSpaceForKeys;
    }

    function calculateKeyHeight() {
        panel.keyHeight = panel.height / numberOfRows();
    }
}
