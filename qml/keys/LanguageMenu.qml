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
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem

import "key_constants.js" as UI
import "languages.js" as Languages

Item {

    property double contentHeight: menuList.contentHeight + units.gu(UI.languageMenuListViewPadding);

    MouseArea {
        width: fullScreenItem.width
        height: fullScreenItem.height

        anchors.centerIn: parent

        onClicked: canvas.languageMenuShown = false
    }

    BorderImage {
        id: name
        anchors.fill: parent
        source: "../images/popover.sci"
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
            text: Languages.languageIdToName(modelData)
            showDivider: modelData != maliit_input_method.enabledLanguages[maliit_input_method.enabledLanguages.length - 1]
            control: CheckBox {
                checked: maliit_input_method.activeLanguage == modelData
                onVisibleChanged: {
                    checked = maliit_input_method.activeLanguage == modelData
                }
                onClicked: {
                    maliit_input_method.activeLanguage = modelData
                    canvas.languageMenuShown = false;
                }
            }
            onClicked: {
                maliit_input_method.activeLanguage = modelData
                canvas.languageMenuShown = false;
            }
        }

        Component {
            id: settingsComp
            Column {
                width: menuList.width
                height: settingsItem.height + settingsDiv.height * 2
                ListItem.ThinDivider { id: settingsDiv }
                ListItem.ThinDivider { }
                ListItem.Standard {
                    id: settingsItem
                    text: i18n.tr("Settings") + "…"
                    showDivider: false
                    onClicked: {
                        Qt.openUrlExternally("settings:///system/language")
                        canvas.languageMenuShown = false;
                        maliit_input_method.hide();
                    }
                }
            }
        }
        footer: greeter_status.greeterActive ? null : settingsComp
    }

}
