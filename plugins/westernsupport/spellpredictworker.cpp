/*
 * Copyright (C) 2014 Canonical, Ltd.
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

#include "spellpredictworker.h"

#include <QDebug>

SpellPredictWorker::SpellPredictWorker(QObject *parent)
    : QObject(parent)
    , m_spellChecker()
    , m_limit(5)
{
}

void SpellPredictWorker::parsePredictionText(const QString& surroundingLeft, const QString& origPreedit)
{
    QStringList list;

    QString preedit = origPreedit;

    // Allow plugins to override certain words such as ('i' -> 'I')
    if(m_overrides.contains(preedit.toLower())) {
        preedit = m_overrides[preedit.toLower()];
        list << preedit;
        // Emit the override corrections instantly so they're always up-to-date
        // as they're often used for short words like 'I'
        Q_EMIT newPredictionSuggestions(origPreedit, list);
    } else if(m_spellChecker.spell(preedit)) {
        // If the user input is spelt correctly add it to the start of the predictions
        list << preedit;
    }

    Q_EMIT newPredictionSuggestions(origPreedit, list);
}

void SpellPredictWorker::setLanguage(QString locale, QString pluginPath)
{
    // locale for secondary layouts I.E., dvorak will be formatted as locale@layout, swiss keyboard as "fr-ch"
    // in this case we want to drop the layout portion
    QStringList tmpLocales = locale.split(QRegularExpression("(@|\\-)"));
    QString baseLocale;
    if (tmpLocales.size() > 1) {
        baseLocale = tmpLocales[0];
        pluginPath = pluginPath.mid(0, pluginPath.size()-(locale.size()-baseLocale.size()));
    } else {
        baseLocale = locale;
    }

    m_spellChecker.setLanguage(baseLocale);
    m_spellChecker.setEnabled(true);
}

void SpellPredictWorker::suggest(const QString& word, int limit)
{
    QStringList suggestions;
    if(!m_spellChecker.spell(word)) {
        suggestions = m_spellChecker.suggest(word, limit);
    }
    // If spelt correctly still send empty suggestions so the plugin knows we
    // have finished processing.
    Q_EMIT newSpellingSuggestions(word, suggestions);
}

void SpellPredictWorker::newSpellCheckWord(QString word)
{
    suggest(word, m_limit);
}

void SpellPredictWorker::addToUserWordList(const QString& word)
{
    m_spellChecker.addToUserWordList(word);
}

void SpellPredictWorker::setSpellCheckLimit(int limit)
{
    m_limit = limit;
}

void SpellPredictWorker::addOverride(const QString& orig, const QString& overridden)
{
    m_overrides[orig] = overridden;
}
