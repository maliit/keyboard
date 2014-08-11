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

#include "spellcheckerworker.h"


SpellCheckerWorker::SpellCheckerWorker(QObject *parent)
    : QObject(parent)
    , m_spellChecker()
    , m_word()
    , m_limit(5)
    , m_processingWords(false)
{
}

void SpellCheckerWorker::suggest(const QString& word, int limit)
{
    QStringList suggestions = m_spellChecker.suggest(word, limit);
    Q_EMIT newSuggestions(word, suggestions);
}

void SpellCheckerWorker::newSpellCheckWord(QString word)
{
    // Run through all the words queued in the event loop
    // so we only fetch suggestions for the latest word
    bool setProcessingWords = false;
    if(m_processingWords == false) {
        setProcessingWords = true;
        m_processingWords = true;
    }
    QCoreApplication::processEvents();
    if(setProcessingWords == true) {
        m_processingWords = false;
    }

    m_word = word;

    if(!m_processingWords) {
        suggest(m_word, m_limit);
    }
}

void SpellCheckerWorker::setLanguage(QString language)
{
    m_spellChecker.setLanguage(language);
}

void SpellCheckerWorker::setLimit(int limit)
{
    m_limit = limit;
}

void SpellCheckerWorker::setEnabled(bool enabled)
{
    m_spellChecker.setEnabled(enabled);
}

void SpellCheckerWorker::updateSpellCheckWord(QString word)
{   
    m_spellChecker.updateWord(word);
}

