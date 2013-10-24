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
//! A word engine that deterministically predicts word candidates, in
//! such a way that it can be used for tests. Does not require
//! Hunspell or Presage. This word engine is for language where all
//! words begin with "a" and ends with "d". So, if we type "abc" we
//! can expect "abcd". If we type "abcd" we expect no candidates, but
//! it is a correct word. If we type "bcd" then we get no candidates,
//! because this word is wrong - it does not begin with "a".


//! \param parent The owner of this instance (optional).
WordEngineProbe::WordEngineProbe(QObject *parent)
    : AbstractWordEngine(parent)
{}


WordEngineProbe::~WordEngineProbe()
{}


//! \brief Returns new candidates.
//! \param text Text model.
WordCandidateList WordEngineProbe::fetchCandidates(Model::Text *text)
{
    const QString preedit(text->preedit());
    WordCandidateList result;
    Model::Text::PreeditFace face;

    if (preedit.left(1) == "a") {
        if (preedit.right(1) == "d") {
            // simulates case when there are no candidates but preedit
            // has incorrect spelling
            face = Model::Text::PreeditDefault;
        } else {
            // simulates case when there are some candidates, preedit
            // spelling correctnes is not important here.
            WordCandidate candidate;
            candidate.rLabel().setText(preedit + "d");
            result << candidate;
            face = Model::Text::PreeditActive;
        }
    } else {
        // simulates case when there are no candidates and preedit has
        // incorrect spelling
        face = Model::Text::PreeditNoCandidates;
    }

    text->setPreeditFace(face);
    return result;
}

}} // namespace MaliitKeyboard::Logic
