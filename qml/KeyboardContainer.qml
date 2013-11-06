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
import "languages/"
import "keys/"
import UbuntuKeyboard 1.0

Item {
    id: panel

    property int keyWidth: 0
    property int keyHeight: 0

    property string activeKeypadState: "NORMAL"

    state: "CHARACTERS"

    function closeExtendedKeys()
    {
        extendedKeysSelector.closePopover();
    }

    Loader {
        id: characterKeypadLoader
        objectName: "characterKeyPadLoader"
        anchors.fill: parent
        asynchronous: false
        source: panel.state === "CHARACTERS" ? internal.characterKeypadSource : internal.symbolKeypadSource
        onLoaded: activeKeypadState = "NORMAL"
    }
    ExtendedKeysSelector {
        id: extendedKeysSelector
        objectName: "extendedKeysSelector"
        anchors.fill: parent
    }

    states: [
        State {
            name: "CHARACTERS"
        },
        State {
            name: "SYMBOLS"
        }
    ]

    QtObject {
        id: internal

        property Item activeKeypad: characterKeypadLoader.item
        property string characterKeypadSource: loadLayout(maliit_input_method.contentType,
                                                          maliit_input_method.systemLanguage,
                                                          maliit_input_method.activeLanguage)
        property string symbolKeypadSource: activeKeypad ? activeKeypad.symbols : ""

        onCharacterKeypadSourceChanged: {
            panel.state = "CHARACTERS";
        }

        /// Returns if the given language is supported
        /// FIXME the possible languages should be checked in C++
        function languageIsSupported(locale)
        {
            var supportedLocales = ["en", "de", "es", "fr", "zh", "pt"];
            return (supportedLocales.indexOf( locale ) > -1);
        }

        /// Returns the relative path to the keyboard QML file for a given language for free text
        function freeTextLanguageKeyboard(language)
        {
            language = language .slice(0,2).toLowerCase();

            if (!languageIsSupported(language)) {
                console.log("Language '"+language+"' not supported - using 'en' instead");
                language = "en";
            }

            if (language === "cs")
                return "languages/cs/Keyboard_cs.qml";
            if (language === "da")
                return "languages/da/Keyboard_da.qml";
            if (language === "de")
                return "languages/de/Keyboard_de.qml";
            if (language === "en")
                return "languages/en/Keyboard_en.qml";
            if (language === "es")
                return "languages/es/Keyboard_es.qml";
            if (language === "fi")
                return "languages/fi/Keyboard_fi.qml";
            if (language === "fr")
                return "languages/fr/Keyboard_fr.qml";
            if (language === "he")
                return "languages/he/Keyboard_he.qml";
            if (language === "it")
                return "languages/it/Keyboard_it.qml";
            if (language === "nl")
                return "languages/nl/Keyboard_nl.qml";
            if (language === "pt")
                return "languages/pt/Keyboard_pt.qml";
            if (language === "ru")
                return "languages/ru/Keyboard_ru.qml";
            if (language === "sv")
                return "languages/sv/Keyboard_sv.qml";
            if (language === "zh")
                return "languages/zh_cn/Keyboard_zh_cn_pinyin.qml";
        }

        function loadLayout(contentType, systemLanguage, activeLanguage)
        {
//            if (contentType === InputMethod.NumberContentType) {
            if (contentType === 1) {
                return "languages/Keyboard_numbers.qml";
            }

//            if (contentType === InputMethod.PhoneNumberContentType) {
            if (contentType === 2) {
                return "languages/Keyboard_telephone.qml";
            }

            var locale = systemLanguage.slice(0,2).toLowerCase();
            if (!languageIsSupported(locale)) {
                console.log("System language '"+locale+"' can't be used in OSK - using 'en' instead")
                locale = "en"
            }

//            if (contentType === InputMethod.EmailContentType) {
            if (contentType === 3) {
                return "languages/"+locale+"/Keyboard_"+locale+"_email.qml";
            }

//            if (contentType === InputMethod.UrlContentType) {
            if (contentType === 4) {
                return "languages/"+locale+"/Keyboard_"+locale+"_url_search.qml";
            }

            // FreeTextContentType used as fallback
            return freeTextLanguageKeyboard(activeLanguage);
        }
    }
}
