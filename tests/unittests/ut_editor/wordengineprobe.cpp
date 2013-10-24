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

#include "wordengineprobe.h"

namespace MaliitKeyboard {
namespace Logic {

//! \class WordEngineProbe
//! A word engine that deterministcally predicts word candidates, in such a
//! way that it can be used for tests. Does not require Hunspell or Presage.


//! \param parent The owner of this instance (optional).
WordEngineProbe::WordEngineProbe(QObject *parent)
    : AbstractWordEngine(parent)
    , candidates()
{}


WordEngineProbe::~WordEngineProbe()
{}

void WordEngineProbe::addSpellingCandidate(const QString &text,
                                           const QString &word)
{
    candidates.insert(text, word);
}

//! \brief Returns new candidates.
//! \param text Preedit of text model is reversed and emitted as only word
//!             candidate. Special characters (e.g., punctuation) are skipped.
WordCandidateList WordEngineProbe::fetchCandidates(Model::Text *text)
{
    WordCandidateList result;

    QHash<QString, QString>::const_iterator candidate = candidates.constFind(text->preedit());
    if (candidate != candidates.constEnd()) {
        text->setPrimaryCandidate(candidate.value());

        WordCandidate word_candidate(WordCandidate::SourceSpellChecking, candidate.value());
        result.append(word_candidate);
    }

    return result;
}

}} // namespace MaliitKeyboard
