/*
 * Copyright 2021 Rodney Dawes
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
import QtQuick 2.9
import QtGraphicalEffects 1.0

import MaliitKeyboard 2.0

Item {
    id: iconRoot

    property string name

    property alias color: iconOverlay.color

    property alias source: iconImage.source

    Connections {
        target: Theme
        onThemeChanged: {
            // Update the sources to try and force a redraw
            iconOverlay.source = undefined;
            iconImage.source = iconRoot.source;
            iconOverlay.source = iconImage;
        }
    }

    Image {
        id: iconImage
        source: iconRoot.name ? "image://icon/%1".arg(iconRoot.name) : ""
        sourceSize.width: width
        sourceSize.height: height
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        cache: false
        visible: false
    }
    ColorOverlay {
        id: iconOverlay
        cached: iconImage.cache
        anchors.fill: iconImage
        source: iconImage
        visible: iconRoot.visible
    }
}
