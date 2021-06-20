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

#include "chineselanguagefeatures.h"

ChineseLanguageFeatures::ChineseLanguageFeatures(QObject *parent) :
    QObject(parent)
{
}

ChineseLanguageFeatures::~ChineseLanguageFeatures() = default;

bool ChineseLanguageFeatures::alwaysShowSuggestions() const
{
    // Pinyin characters can only be entered via suggestions, so we ignore
    // hints that would otherwise disable them.
    return true;
}

bool ChineseLanguageFeatures::autoCapsAvailable() const
{
    // Automatic switching to capital letters doen't make sense when
    // inputting Pinyin
    return false;
}

bool ChineseLanguageFeatures::activateAutoCaps(const QString &preedit) const
{
    Q_UNUSED(preedit)
    return false;
}

QString ChineseLanguageFeatures::appendixForReplacedPreedit(const QString &preedit) const
{
    if (isSeparator(preedit.right(1)) && contentType() != Maliit::EmailContentType && contentType() != Maliit::UrlContentType) {
        return QStringLiteral(" ");
    }

    return QString();
}

bool ChineseLanguageFeatures::isSeparator(const QString &text) const
{
    static const QString separators = QStringLiteral("。、,!?:;.…\r\n");

    if (text.isEmpty()) {
        return false;
    }

    if (separators.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool ChineseLanguageFeatures::isSymbol(const QString &text) const
{
    static const QString symbols = QStringLiteral(R"(*#+=()@~/\€£$¥₹%<>[]`^|_—–•§{}¡¿«»"“”„&0123456789)");

    if (text.isEmpty()) {
        return false;
    }

    if (symbols.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool ChineseLanguageFeatures::ignoreSimilarity() const
{
    return true;
}

bool ChineseLanguageFeatures::wordEngineAvailable() const
{
    return true;
}

QString ChineseLanguageFeatures::fullStopSequence() const
{
    return QStringLiteral("。");
}

bool ChineseLanguageFeatures::shouldDelayCandidateCommit() const
{
    return true;
}

std::size_t ChineseLanguageFeatures::primaryCandidateIndex() const
{
    return 2;
}
