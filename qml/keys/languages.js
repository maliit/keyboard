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
    if (languageId == "ar")         return i18n.tr("Arabic");
    if (languageId == "az")         return i18n.tr("Azerbaijani");
    if (languageId == "bg")         return i18n.tr("Bulgarian");
    if (languageId == "bs")         return i18n.tr("Bosnian");
    if (languageId == "ca")         return i18n.tr("Catalan");
    if (languageId == "cs")         return i18n.tr("Czech");
    if (languageId == "da")         return i18n.tr("Danish");
    if (languageId == "de")         return i18n.tr("German");
    if (languageId == "emoji")      return i18n.tr("Emoji");
    if (languageId == "el")         return i18n.tr("Greek");
    if (languageId == "en")         return i18n.tr("English");
    if (languageId == "en@dv")      return i18n.tr("English\n(Dvorak)");
    if (languageId == "eo")         return i18n.tr("Esperanto");
    if (languageId == "es")         return i18n.tr("Spanish");
    if (languageId == "fa")         return i18n.tr("Persian");
    if (languageId == "fi")         return i18n.tr("Finnish");
    if (languageId == "fr")         return i18n.tr("French");
    if (languageId == "fr-ch")      return i18n.tr("French\n(Swiss)");
    if (languageId == "gd")         return i18n.tr("Scottish Gaelic");
    if (languageId == "he")         return i18n.tr("Hebrew");
    if (languageId == "hr")         return i18n.tr("Croatian");
    if (languageId == "hu")         return i18n.tr("Hungarian");
    if (languageId == "is")         return i18n.tr("Icelandic");
    if (languageId == "it")         return i18n.tr("Italian");
    if (languageId == "ja")         return i18n.tr("Japanese");
    if (languageId == "lt")         return i18n.tr("Lithuanian");
    if (languageId == "lv")         return i18n.tr("Latvian");
    if (languageId == "ko")         return i18n.tr("Korean");
    if (languageId == "nl")         return i18n.tr("Dutch");
    if (languageId == "nb")         return i18n.tr("Norwegian");
    if (languageId == "pl")         return i18n.tr("Polish");
    if (languageId == "pt")         return i18n.tr("Portuguese");
    if (languageId == "ro")         return i18n.tr("Romanian");
    if (languageId == "ru")         return i18n.tr("Russian");
    if (languageId == "sl")         return i18n.tr("Slovenian");
    if (languageId == "sr")         return i18n.tr("Serbian");
    if (languageId == "sv")         return i18n.tr("Swedish");
    if (languageId == "tr")         return i18n.tr("Turkish");
    if (languageId == "uk")         return i18n.tr("Ukrainian");
    if (languageId == "zh-hans")         return i18n.tr("Chinese\n(Pinyin)");
    if (languageId == "zh-hant")         return i18n.tr("Chinese\n(Chewing)");

    // fallback
    return languageId.charAt(0).toUpperCase() + languageId.slice(1);
}
