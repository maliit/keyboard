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
import QtQuick.Window 2.0

Item {
    id: orientationHelper

    property string __rotationString: Screen.angleBetween(Screen.primaryOrientation, Screen.orientation).toString();

    rotation: Screen.angleBetween(Screen.primaryOrientation, Screen.orientation);

    anchors.fill: parent

    Item {
        state: __rotationString

        states: [
            State {
                name: "270"
                PropertyChanges {
                    target: orientationHelper
                    anchors {
                        leftMargin: (parent.width - parent.height) / 2
                        rightMargin: anchors.leftMargin
                        topMargin: -anchors.leftMargin
                        bottomMargin: anchors.topMargin
                    }
                }
            },
            State {
                name: "90"
                PropertyChanges {
                    target: orientationHelper
                    anchors {
                        leftMargin: (parent.width - parent.height) / 2
                        rightMargin: anchors.leftMargin
                        topMargin: -anchors.leftMargin
                        bottomMargin: anchors.topMargin
                    }
                }
            }
        ]
   }
}
