/*
 * Copyright 2014 Canonical Ltd.
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

#include "thailanguagefeatures.h"

ThaiLanguageFeatures::ThaiLanguageFeatures(QObject *parent) :
    QObject(parent)
{
}

ThaiLanguageFeatures::~ThaiLanguageFeatures()
{
}

bool ThaiLanguageFeatures::alwaysShowSuggestions() const
{
    return false;
}

bool ThaiLanguageFeatures::autoCapsAvailable() const
{
    return false;
}

bool ThaiLanguageFeatures::activateAutoCaps(const QString &preedit) const
{
    Q_UNUSED(preedit)
    return false;
}

QString ThaiLanguageFeatures::appendixForReplacedPreedit(const QString &preedit) const
{
    Q_UNUSED(preedit)
    return QString("");
}

bool ThaiLanguageFeatures::isSeparator(const QString &text) const
{
    static const QString separators = QString::fromUtf8("。、,!?:;.\r\n");

    if (text.isEmpty()) {
        return false;
    }

    if (separators.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool ThaiLanguageFeatures::isSymbol(const QString &text) const
{
    static const QString symbols = QString::fromUtf8("*#+=()@~/\\€£$¥₹%<>[]`^|_§{}¡¿«»\"“”„&0123456789");

    if (text.isEmpty()) {
        return false;
    }

    if (symbols.contains(text.right(1))) {
        return true;
    }

    return false;
}
