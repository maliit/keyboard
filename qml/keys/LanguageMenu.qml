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
import Ubuntu.Components.ListItems 1.3 as ListItems

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
    
    Rectangle{
        color: fullScreenItem.theme.charKeyColor
        anchors.fill: parent
        anchors.margins: units.gu(0.1)
        radius: units.gu(0.8)
        border{
            width: units.gu(0.1)
            color: fullScreenItem.theme.popupBorderColor
        }
    }

    ListView {
        id: menuList
        anchors.centerIn: parent
        width: parent.width - units.gu(UI.languageMenuListViewPadding)
        height: parent.height - units.gu(UI.languageMenuListViewPadding)
        interactive: true
        clip: true

        model: maliit_input_method.enabledLanguages
        
        delegate: ListItem {
                highlightColor: fullScreenItem.theme.charKeyPressedColor
                divider{
                    visible: modelData != maliit_input_method.enabledLanguages[maliit_input_method.enabledLanguages.length - 1]
                }
                
                ListItemLayout {
                    title.text: Languages.languageIdToName(modelData)
                    title.color: fullScreenItem.theme.fontColor
                    title.elide: Text.ElideRight
                    title.maximumLineCount: 2
                    CheckBox {
                        SlotsLayout.position: SlotsLayout.Trailing;
                        checked: maliit_input_method.activeLanguage == modelData
                        onVisibleChanged: {
                            checked = maliit_input_method.activeLanguage == modelData
                        }
                        onClicked: {
                            maliit_input_method.activeLanguage = modelData
                            canvas.languageMenuShown = false;
                        }
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
                
                ListItem {
                    id: settingsDiv
                    height: units.dp(2)
                    divider{
                        height: units.dp(2)
                    }
                }
                ListItem {
                    id: settingsItem
                    highlightColor: fullScreenItem.theme.charKeyPressedColor
                    divider.visible: false
                    ListItemLayout {
                       title.text: i18n.tr("Settings") + "…"
                       title.color: fullScreenItem.theme.fontColor
                   }
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
