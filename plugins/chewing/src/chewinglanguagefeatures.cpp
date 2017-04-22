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

#include "chewinglanguagefeatures.h"

ChewingLanguageFeatures::ChewingLanguageFeatures(QObject *parent) :
    QObject(parent)
{
}

ChewingLanguageFeatures::~ChewingLanguageFeatures() = default;

bool ChewingLanguageFeatures::alwaysShowSuggestions() const
{
    // Chewing characters can only be entered via suggestions, so we ignore
    // hints that would otherwise disable them.
    return true;
}

bool ChewingLanguageFeatures::autoCapsAvailable() const
{
    // Automatic switching to capital letters doesn't make sense when 
    // inputting Chewing
    return false;
}

bool ChewingLanguageFeatures::activateAutoCaps(const QString &preedit) const
{
    Q_UNUSED(preedit)
    return false;
}

QString ChewingLanguageFeatures::appendixForReplacedPreedit(const QString &preedit) const
{
    if (preedit.isEmpty())
        return QString();

    return QStringLiteral(" ");
}

bool ChewingLanguageFeatures::isSeparator(const QString &text) const
{
    static const QString separators = QStringLiteral("。、!?:…\r\n");

    if (text.isEmpty()) {
        return false;
    }

    if (separators.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool ChewingLanguageFeatures::isSymbol(const QString &text) const
{
    static const QString symbols = QStringLiteral(R"(*#+=()@~\€£$¥₹%<>[]`^|_—–•§{}¡¿«»"“”„&)");

    if (text.isEmpty()) {
        return false;
    }

    if (symbols.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool ChewingLanguageFeatures::ignoreSimilarity() const
{
    return true;
}

bool ChewingLanguageFeatures::wordEngineAvailable() const
{
    return true;
}

QString ChewingLanguageFeatures::fullStopSequence() const
{
    return QStringLiteral("。");
}

bool ChewingLanguageFeatures::restorePreedit() const
{
    return false;
}

bool ChewingLanguageFeatures::commitOnSpace() const
{
    return false;
}

bool ChewingLanguageFeatures::showPrimaryInPreedit() const
{
    return true;
}

