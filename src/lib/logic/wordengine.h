/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 *
 * Contact: Mohammad Anwari <Mohammad.Anwari@nokia.com>
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

#ifndef MALIIT_KEYBOARD_WORDENGINE_H
#define MALIIT_KEYBOARD_WORDENGINE_H

#include "logic/abstractwordengine.h"
#include "logic/abstractlanguagefeatures.h"
#include "languageplugininterface.h"


namespace MaliitKeyboard {
namespace Logic {

class WordEnginePrivate;

class WordEngine
    : public AbstractWordEngine
{
    Q_OBJECT
    Q_DISABLE_COPY(WordEngine)
    Q_DECLARE_PRIVATE(WordEngine)

public:
    explicit WordEngine(QObject *parent = nullptr);
    ~WordEngine() override;

    //! \reimp
    bool isEnabled() const override;
    void setWordPredictionEnabled(bool enabled) override;

    void addToUserDictionary(const QString &word) override;
    void setSpellcheckerEnabled(bool enabled) override;
    void setAutoCorrectEnabled(bool enabled) override;
    void clearCandidates() override;
    //! \reimp_end

    void appendToCandidates(WordCandidateList *candidates,
                                        WordCandidate::Source source,
                                        const QString &candidate);

    Q_SLOT void onWordCandidateSelected(QString word) override;
    Q_SLOT void onLanguageChanged(const QString& pluginPath, const QString& languageId) override;

    Q_SLOT void updateQmlCandidates(QStringList qmlCandidates) override;
    Q_SLOT void newSpellingSuggestions(QString word, QStringList suggestions,
                                       int strategy = UpdateCandidateListStrategy::ClearWhenNeeded);
    Q_SLOT void newPredictionSuggestions(QString word, QStringList suggestions,
                                         int strategy = UpdateCandidateListStrategy::ClearWhenNeeded);

    AbstractLanguageFeatures* languageFeature() override;

private:
    //! \reimp
    void fetchCandidates(Model::Text *text) override;
    //! \reimp_end

    //! Calculate the primary candidate if there is not any.
    void calculatePrimaryCandidate();
    //! Calculate the primary candidate unconditionally.
    void forceCalculatePrimaryCandidate();
    void calculatePrimaryCandidateImpl();
    bool similarWords(QString word1, QString word2);

    const QScopedPointer<WordEnginePrivate> d_ptr;

    QMutex suggestionMutex;
};

}} // namespace Logic, MaliitKeyboard

#endif // MALIIT_KEYBOARD_WORDENGINE_H
