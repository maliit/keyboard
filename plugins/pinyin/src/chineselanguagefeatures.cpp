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

ChineseLanguageFeatures::~ChineseLanguageFeatures()
{
}

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
    Q_UNUSED(preedit)
    return QString("");
}
