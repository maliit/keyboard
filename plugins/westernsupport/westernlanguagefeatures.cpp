/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 Openismus GmbH
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Nokia Corporation nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "westernlanguagefeatures.h"

#include <QtCore>

WesternLanguageFeatures::WesternLanguageFeatures(QObject *parent) :
    QObject(parent)
{
}

WesternLanguageFeatures::~WesternLanguageFeatures() = default;

bool WesternLanguageFeatures::alwaysShowSuggestions() const
{
    return false;
}

bool WesternLanguageFeatures::autoCapsAvailable() const
{
    return true;
}

bool WesternLanguageFeatures::activateAutoCaps(const QString &preedit) const
{

    static const QString sentenceBreak = QString::fromUtf8("!.?\r\n");

    if (preedit.isEmpty()) {
        return false;
    }

    if (sentenceBreak.contains(preedit.mid(preedit.length() - 2, 1)) && preedit.at(preedit.count() - 1).isSpace()) {
        return true;
    }

    return false;
}

QString WesternLanguageFeatures::appendixForReplacedPreedit(const QString &preedit) const
{
    if (preedit.isEmpty())
        return QString();

    return QStringLiteral(" ");
}

bool WesternLanguageFeatures::isSeparator(const QString &text) const
{
    static const QString separators = QStringLiteral(",.!?:;…\r\n");

    if (text.isEmpty()) {
        return false;
    }

    if (separators.contains(text.right(1))) {
        return true;
    }

    return false;
}

bool WesternLanguageFeatures::isSymbol(const QString &text) const
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

bool WesternLanguageFeatures::ignoreSimilarity() const
{
    return false;
}

bool WesternLanguageFeatures::wordEngineAvailable() const
{
    return true;
}

bool WesternLanguageFeatures::restorePreedit() const
{
    return true;
}
