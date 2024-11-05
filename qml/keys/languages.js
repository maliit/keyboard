/*
 * Copyright 2016 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

function languageIdToName(languageId)
{
    if (languageId == "ar")              return Gettext.qsTr("Arabic");
    if (languageId == "az")              return Gettext.qsTr("Azerbaijani");
    if (languageId == "be")              return Gettext.qsTr("Belarusian");
    if (languageId == "bg")              return Gettext.qsTr("Bulgarian");
    if (languageId == "bs")              return Gettext.qsTr("Bosnian");
    if (languageId == "ca")              return Gettext.qsTr("Catalan");
    if (languageId == "cs")              return Gettext.qsTr("Czech");
    if (languageId == "da")              return Gettext.qsTr("Danish");
    if (languageId == "de")              return Gettext.qsTr("German");
    if (languageId == "de@terminal")     return Gettext.qsTr("German\n(Terminal)");
    if (languageId == "el")              return Gettext.qsTr("Greek");
    if (languageId == "en")              return Gettext.qsTr("English");
    if (languageId == "en@dv")           return Gettext.qsTr("English\n(Dvorak)");
    if (languageId == "eo")              return Gettext.qsTr("Esperanto");
    if (languageId == "es")              return Gettext.qsTr("Spanish");
    if (languageId == "fa")              return Gettext.qsTr("Persian");
    if (languageId == "fi")              return Gettext.qsTr("Finnish");
    if (languageId == "fr")              return Gettext.qsTr("French");
    if (languageId == "fr-ch")           return Gettext.qsTr("French\n(Swiss)");
    if (languageId == "gd")              return Gettext.qsTr("Scottish Gaelic");
    if (languageId == "he")              return Gettext.qsTr("Hebrew");
    if (languageId == "hr")              return Gettext.qsTr("Croatian");
    if (languageId == "hu")              return Gettext.qsTr("Hungarian");
    if (languageId == "is")              return Gettext.qsTr("Icelandic");
    if (languageId == "it")              return Gettext.qsTr("Italian");
    if (languageId == "ja")              return Gettext.qsTr("Japanese");
    if (languageId == "lt")              return Gettext.qsTr("Lithuanian");
    if (languageId == "lv")              return Gettext.qsTr("Latvian");
    if (languageId == "mk")              return Gettext.qsTr("Macedonian");
    if (languageId == "ko")              return Gettext.qsTr("Korean");
    if (languageId == "nl")              return Gettext.qsTr("Dutch");
    if (languageId == "nb")              return Gettext.qsTr("Norwegian");
    if (languageId == "pl")              return Gettext.qsTr("Polish");
    if (languageId == "pt")              return Gettext.qsTr("Portuguese");
    if (languageId == "ro")              return Gettext.qsTr("Romanian");
    if (languageId == "ru")              return Gettext.qsTr("Russian");
    if (languageId == "sl")              return Gettext.qsTr("Slovenian");
    if (languageId == "sr")              return Gettext.qsTr("Serbian");
    if (languageId == "sv")              return Gettext.qsTr("Swedish");
    if (languageId == "th")              return Gettext.qsTr("Thai");
    if (languageId == "tr")              return Gettext.qsTr("Turkish");
    if (languageId == "uk")              return Gettext.qsTr("Ukrainian");
    if (languageId == "zh-hans")         return Gettext.qsTr("Chinese\n(Pinyin)");
    if (languageId == "zh-hant")         return Gettext.qsTr("Chinese\n(Chewing)");

    // fallback
    return languageId.charAt(0).toUpperCase() + languageId.slice(1);
}
