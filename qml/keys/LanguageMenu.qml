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

        model: maliit_input_method.enabledLanguages

        delegate: ListItem.Standard {
            text: languageIdToName(modelData)
            onClicked: {
                maliit_input_method.activeLanguage = modelData
                canvas.languageMenuShown = false;
            }
         }
    }

    function languageIdToName(languageId)
    {
        if (languageId == "cs")         return i18n.tr("Czech");
        if (languageId == "da")         return i18n.tr("Danish");
        if (languageId == "de")         return i18n.tr("German");
        if (languageId == "en")         return i18n.tr("English");
        if (languageId == "es")         return i18n.tr("Spanish");
        if (languageId == "fi")         return i18n.tr("Finnish");
        if (languageId == "fr")         return i18n.tr("French");
        if (languageId == "he")         return i18n.tr("Hebrew");
        if (languageId == "it")         return i18n.tr("Italian");
        if (languageId == "nl")         return i18n.tr("Dutch");
        if (languageId == "pt")         return i18n.tr("Portuguese");
        if (languageId == "sv")         return i18n.tr("Swedish");
        if (languageId == "zh")         return i18n.tr("Chinese - Pinyin");
    }

}
