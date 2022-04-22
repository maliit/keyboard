/*
 * Copyright 2013 Canonical Ltd.
 * Copyright 2019 Jan Arne Petersen
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

import QtQuick.Controls 2.12

import MaliitKeyboard 2.0

import "languages.js" as Languages

Menu {

    modal: true

    Repeater {
        model: Keyboard.enabledLanguages

        delegate: MenuItem {
            text: Languages.languageIdToName(modelData)
            checkable: true
            autoExclusive: true
            checked: Keyboard.activeLanguage == modelData
            onClicked: {
                Keyboard.activeLanguage = modelData
                canvas.languageMenu.close()
            }
        }
    }
    MenuItem {
        text: Gettext.qsTr("Emoji")
        onClicked: {
            keypad.state = "EMOJI";
            canvas.languageMenu.close();
        }
    }
    MenuSeparator {
    }
    MenuItem {
        id: settingsItem
        text: Gettext.qsTr("Settings") + "…"
        onClicked: {
            Keyboard.showSystemSettings();
            canvas.languageMenu.close();
        }
    }
}
