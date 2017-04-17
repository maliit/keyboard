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
    if (languageId == "ar")         return qsTr("Arabic");
    if (languageId == "az")         return qsTr("Azerbaijani");
    if (languageId == "bs")         return qsTr("Bosnian");
    if (languageId == "ca")         return qsTr("Catalan");
    if (languageId == "cs")         return qsTr("Czech");
    if (languageId == "da")         return qsTr("Danish");
    if (languageId == "de")         return qsTr("German");
    if (languageId == "emoji")      return qsTr("Emoji");
    if (languageId == "el")         return qsTr("Greek");
    if (languageId == "en")         return qsTr("English");
    if (languageId == "eo")         return qsTr("Esperanto");
    if (languageId == "es")         return qsTr("Spanish");
    if (languageId == "fa")         return qsTr("Persian");
    if (languageId == "fi")         return qsTr("Finnish");
    if (languageId == "fr")         return qsTr("French");
    if (languageId == "gd")         return qsTr("Scottish Gaelic");
    if (languageId == "he")         return qsTr("Hebrew");
    if (languageId == "hr")         return qsTr("Croatian");
    if (languageId == "hu")         return qsTr("Hungarian");
    if (languageId == "is")         return qsTr("Icelandic");
    if (languageId == "it")         return qsTr("Italian");
    if (languageId == "ja")         return qsTr("Japanese");
    if (languageId == "lv")         return qsTr("Latvian");
    if (languageId == "ko")         return qsTr("Korean");
    if (languageId == "nl")         return qsTr("Dutch");
    if (languageId == "nb")         return qsTr("Norwegian");
    if (languageId == "pl")         return qsTr("Polish");
    if (languageId == "pt")         return qsTr("Portuguese");
    if (languageId == "ro")         return qsTr("Romanian");
    if (languageId == "ru")         return qsTr("Russian");
    if (languageId == "sl")         return qsTr("Slovenian");
    if (languageId == "sr")         return qsTr("Serbian");
    if (languageId == "sv")         return qsTr("Swedish");
    if (languageId == "uk")         return qsTr("Ukrainian");
    if (languageId == "zh-hans")         return qsTr("Chinese\n(Pinyin)");
    if (languageId == "zh-hant")         return qsTr("Chinese\n(Chewing)");

    // fallback
    return languageId.charAt(0).toUpperCase() + languageId.slice(1);
}
