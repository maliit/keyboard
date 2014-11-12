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
import QtMultimedia 5.0
import QtQuick.Window 2.0
import "languages/"
import "keys/"
import UbuntuKeyboard 1.0
import QtFeedback 5.0

Item {
    id: panel

    property int keyWidth: 0
    property int keyHeight: 0

    property bool autoCapsTriggered: false
    property bool delayedAutoCaps: false

    property string activeKeypadState: "NORMAL"
    property alias popoverEnabled: extendedKeysSelector.enabled

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
        onLoaded: {
            if (delayedAutoCaps) {
                activeKeypadState = "SHIFTED";
                delayedAutoCaps = false;
            } else {
                activeKeypadState = "NORMAL";
            }
        }
    }

    ExtendedKeysSelector {
        id: extendedKeysSelector
        objectName: "extendedKeysSelector"
        anchors.fill: parent
    }

    Magnifier {
        id: magnifier
        shown: false
    }

    SoundEffect {
        id: audioFeedback
        source: maliit_input_method.audioFeedbackSound
    }

    Connections {
        target: maliit_input_method
        onAudioFeedbackSoundChanged: audioFeedback.source = sound;
    }

    HapticsEffect {
        id: pressEffect
        attackIntensity: 0.0
        attackTime: 50
        intensity: 1.0
        duration: 10
        fadeTime: 50
        fadeIntensity: 0.0
    }

    states: [
        State {
            name: "CHARACTERS"
        },
        State {
            name: "SYMBOLS"
        }
    ]

    onStateChanged: {
        maliit_input_method.keyboardState = state
    }

    QtObject {
        id: internal

        property Item activeKeypad: characterKeypadLoader.item
        property string characterKeypadSource: loadLayout(maliit_input_method.contentType,
                                                          maliit_input_method.activeLanguage)
        property string symbolKeypadSource: activeKeypad ? activeKeypad.symbols : ""

        onCharacterKeypadSourceChanged: {
            panel.state = "CHARACTERS";
        }

        /// Returns if the given language is supported
        /// FIXME the possible languages should be checked in C++
        function languageIsSupported(locale)
        {
            var supportedLocales = [
                "ar",
                "az",
                "bs",
                "ca",
                "cs",
                "da",
                "de",
                "en",
                "es",
                "fi",
                "fr",
                "he",
                "hr",
                "hu",
                "it",
                "nl",
                "pl",
                "pt",
                "ru",
                "sr",
                "sv",
                "zh",
            ];
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

            if (language === "ar")
                return "lib/ar/Keyboard_ar.qml";
            if (language === "az")
                return "lib/az/Keyboard_az.qml";
            if (language === "bs")
                return "lib/bs/Keyboard_bs.qml";
            if (language === "ca")
                return "lib/ca/Keyboard_ca.qml";
            if (language === "cs")
                return "lib/cs/Keyboard_cs.qml";
            if (language === "da")
                return "lib/da/Keyboard_da.qml";
            if (language === "de")
                return "lib/de/Keyboard_de.qml";
            if (language === "en")
                return "lib/en/Keyboard_en.qml";
            if (language === "es")
                return "lib/es/Keyboard_es.qml";
            if (language === "fi")
                return "lib/fi/Keyboard_fi.qml";
            if (language === "fr")
                return "lib/fr/Keyboard_fr.qml";
            if (language === "he")
                return "lib/he/Keyboard_he.qml";
            if (language === "hr")
                return "lib/hr/Keyboard_hr.qml";
            if (language === "hu")
                return "lib/hu/Keyboard_hu.qml";
            if (language === "it")
                return "lib/it/Keyboard_it.qml";
            if (language === "nl")
                return "lib/nl/Keyboard_nl.qml";
            if (language === "pl")
                return "lib/pl/Keyboard_pl.qml";
            if (language === "pt")
                return "lib/pt/Keyboard_pt.qml";
            if (language === "ru")
                return "lib/ru/Keyboard_ru.qml";
            if (language === "sr")
                return "lib/sr/Keyboard_sr.qml";
            if (language === "sv")
                return "lib/sv/Keyboard_sv.qml";
            if (language === "zh")
                return "lib/zh/Keyboard_zh_cn_pinyin.qml";
        }

        function loadLayout(contentType, activeLanguage)
        {
            //            if (contentType === InputMethod.NumberContentType) {
            if (contentType === 1) {
                return "languages/Keyboard_numbers.qml";
            }

            //            if (contentType === InputMethod.PhoneNumberContentType) {
            if (contentType === 2) {
                return "languages/Keyboard_telephone.qml";
            }

            var locale = activeLanguage.slice(0,2).toLowerCase();
            if (!languageIsSupported(locale)) {
                console.log("System language '"+locale+"' can't be used in OSK - using 'en' instead")
                locale = "en"
            }

            //            if (contentType === InputMethod.EmailContentType) {
            if (contentType === 3) {
                return "lib/"+locale+"/Keyboard_"+locale+"_email.qml";
            }

            //            if (contentType === InputMethod.UrlContentType) {
            if (contentType === 4) {
                return "lib/"+locale+"/Keyboard_"+locale+"_url_search.qml";
            }

            // FreeTextContentType used as fallback
            return freeTextLanguageKeyboard(activeLanguage);
        }
    }
}
