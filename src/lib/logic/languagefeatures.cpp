/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 Openismus GmbH
 *
 * Contact: maliit-discuss@lists.maliit.org
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

#include "languagefeatures.h"

#include <QtCore>

namespace MaliitKeyboard {
namespace Logic {

LanguageFeatures::LanguageFeatures(QObject *parent) :
    AbstractLanguageFeatures(parent)
{
}

LanguageFeatures::~LanguageFeatures()
{
}

bool LanguageFeatures::activateAutoCaps(const QString &preedit) const
{
    static const QString sentenceBreak = QString::fromUtf8("!.?:");

    if (preedit.isEmpty()) {
        return false;
    }

    if (sentenceBreak.contains(preedit.right(1))) {
        return true;
    }

    return false;
}

QString LanguageFeatures::appendixForReplacedPreedit(const QString &preedit) const
{
    // Get punctation chars at the end of preedit string
    QString appendix;
    for (int i = preedit.size() - 1; i >= 0; i--)
    {
        if (not preedit.at(i).isPunct()) {
            appendix = preedit.right(preedit.size() - i - 1);
            break;
        }
    }

    appendix.append(" ");
    return appendix;
}

} // namespace Logic
} // namespace MaliitKeyboard
