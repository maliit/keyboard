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

#ifndef SPELLPREDICTWORKER_H
#define SPELLPREDICTWORKER_H

#include "spellchecker.h"
#include "languageplugininterface.h"

#include <QObject>
#include <QStringList>
#include <QMap>

class SpellPredictWorker : public QObject
{
    Q_OBJECT

public:
    SpellPredictWorker(QObject *parent = 0);
    void suggest(const QString& word, int limit);

public slots:
    void parsePredictionText(const QString& surroundingLeft, const QString& preedit);
    void newSpellCheckWord(QString word);
    void setLanguage(QString language, QString pluginPath);
    void setSpellCheckLimit(int limit);
    void addToUserWordList(const QString& word);
    void addOverride(const QString& orig, const QString& overridden);

signals:
    void newSpellingSuggestions(QString word, QStringList suggestions,
                                int strategy = UpdateCandidateListStrategy::ClearWhenNeeded);
    void newPredictionSuggestions(QString word, QStringList suggestions,
                                  int strategy = UpdateCandidateListStrategy::ClearWhenNeeded);

private:
    SpellChecker m_spellChecker;
    int m_limit;
    QMap<QString, QString> m_overrides;
};

#endif // SPELLPREDICTWORKER_H
