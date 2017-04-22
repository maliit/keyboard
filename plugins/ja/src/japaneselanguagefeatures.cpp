/*
 * Copyright 2015 Canonical Ltd.
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

#include "japaneselanguagefeatures.h"

JapaneseLanguageFeatures::JapaneseLanguageFeatures(QObject *parent) :
    QObject(parent)
{
}

JapaneseLanguageFeatures::~JapaneseLanguageFeatures() = default;

bool JapaneseLanguageFeatures::alwaysShowSuggestions() const
{
    // Japanese characters can only be entered via suggestions.
    return true;
}

bool JapaneseLanguageFeatures::autoCapsAvailable() const
{
    return false;
}

bool JapaneseLanguageFeatures::activateAutoCaps(const QString &preedit) const
{
    Q_UNUSED(preedit)
    return false;
}

QString JapaneseLanguageFeatures::appendixForReplacedPreedit(const QString &preedit) const
{
    Q_UNUSED(preedit)
    // auto add a character in any event?
    return QString();
}

bool JapaneseLanguageFeatures::isSeparator(const QString &text) const
{
    static const QString separators = QStringLiteral("。、,!?:;.\r\n");

    if (text.isEmpty()) {
        return false;
    }

    if (separators.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool JapaneseLanguageFeatures::isSymbol(const QString &text) const
{
    Q_UNUSED(text)

    return false;
}

bool JapaneseLanguageFeatures::ignoreSimilarity() const
{
    return true;
}

bool JapaneseLanguageFeatures::wordEngineAvailable() const
{
    return true;
}

bool JapaneseLanguageFeatures::enablePreeditAtInsertion() const
{
    return true;
}

bool JapaneseLanguageFeatures::restorePreedit() const
{
    return false;
}
