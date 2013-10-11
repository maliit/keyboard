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

Item {

    Rectangle {
        anchors.fill: parent
        color: "lightGray"
    }

    // tempor., will come from settings
    ListModel {
        id: availableLanguagesModel
        ListElement { language: "English - US"; languageId: "en_us"; }
        ListElement { language: "Spanish"; languageId: "es"; }
        ListElement { language: "Portuguese"; languageId: "pt"; }
        ListElement { language: "French"; languageId: "fr"; }
        ListElement { language: "German"; languageId: "de"; }
        ListElement { language: "Chinese - Pinyin"; languageId: "zh_cn_pinyin"; }
    }

    ListView {
        id: menuList
        anchors.fill: parent
        interactive: false

        model: availableLanguagesModel

        delegate: ListItem.Standard {
            text: language
            onClicked: {
                canvas.layoutId = languageId;
                canvas.languageMenuShown = false;
            }
         }
    }
}
