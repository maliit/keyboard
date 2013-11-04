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
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

import "key_constants.js" as UI

Item {

    MouseArea {
        width: fullScreenItem.width
        height: fullScreenItem.height

        anchors.centerIn: parent

        onClicked: canvas.languageMenuShown = false
    }

    BorderImage {
        id: name
        anchors.fill: parent
        source: "../images/popover@27.png"

        property int __corner: units.gu(UI.languageMenuCorner)

        border.left: __corner; border.top: __corner;
        border.right: __corner; border.bottom: __corner;
    }

    ListView {
        id: menuList
        anchors.centerIn: parent
        width: parent.width - units.gu(UI.languageMenuListViewPadding)
        height: parent.height - units.gu(UI.languageMenuListViewPadding)
        interactive: true
        clip: true

        model: canvas.enabledLanguages

        delegate: ListItem.Standard {
            text: languageIdToName(modelData)
            onClicked: {
                canvas.layoutId = modelData;
                canvas.languageMenuShown = false;
            }
         }
    }

    function languageIdToName(languageId)
    {
        if (languageId == "de")         return qsTr("German");
        if (languageId == "en")         return qsTr("English");
        if (languageId == "es")         return qsTr("Spanish");
        if (languageId == "fr")         return qsTr("French");
        if (languageId == "it")         return qsTr("Italian");
        if (languageId == "nl")         return qsTr("Dutch");
        if (languageId == "pt")         return qsTr("Portuguese");
        if (languageId == "sv")         return qsTr("Swedish");
        if (languageId == "zh")         return qsTr("Chinese - Pinyin");
    }

}
