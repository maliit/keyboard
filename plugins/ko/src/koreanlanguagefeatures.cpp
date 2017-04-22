/*
 * Copyright 2016 Canonical Ltd.
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

#include "koreanlanguagefeatures.h"

KoreanLanguageFeatures::KoreanLanguageFeatures(QObject *parent) :
    QObject(parent)
{
}

KoreanLanguageFeatures::~KoreanLanguageFeatures() = default;

bool KoreanLanguageFeatures::alwaysShowSuggestions() const
{
    return false;
}

bool KoreanLanguageFeatures::autoCapsAvailable() const
{
    return false;
}

bool KoreanLanguageFeatures::activateAutoCaps(const QString &preedit) const
{
    Q_UNUSED(preedit)
    return false;
}

QString KoreanLanguageFeatures::appendixForReplacedPreedit(const QString &preedit) const
{
    if (preedit.isEmpty())
        return QString();

    return QStringLiteral(" ");
}

bool KoreanLanguageFeatures::isSeparator(const QString &text) const
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

bool KoreanLanguageFeatures::isSymbol(const QString &text) const
{
    static const QString symbols = QStringLiteral(R"(*#+=()@~/\€£$¥₹%<>[]`^|_§{}¡¿«»"“”„&0123456789)");

    if (text.isEmpty()) {
        return false;
    }

    if (symbols.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool KoreanLanguageFeatures::ignoreSimilarity() const
{
    return true;
}
bool KoreanLanguageFeatures::wordEngineAvailable() const
{
    return true;
}
