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

#include "wordengine.h"
#include "spellchecker.h"

#ifdef HAVE_PRESAGE
#include <presage.h>
#endif

#ifdef HAVE_PINYIN
#include "pinyinadapter.h"
#include <iostream>
#endif

namespace MaliitKeyboard {
namespace Logic {

namespace {

void appendToCandidates(WordCandidateList *candidates,
                        WordCandidate::Source source,
                        const QString &candidate,
                        bool is_preedit_capitalized)
{
    if (not candidates) {
        return;
    }

    QString changed_candidate(candidate);

    if (not changed_candidate.isEmpty() && is_preedit_capitalized) {
        changed_candidate[0] = changed_candidate.at(0).toUpper();
    }

    WordCandidate word_candidate(source, changed_candidate);

    if (not candidates->contains(word_candidate)) {
        candidates->append(word_candidate);
    }
}

} // namespace

//! \class WordEngine
//! \brief Provides error correction (based on Hunspell) and word
//! prediction (based on Presage).

//! \internal
#ifdef HAVE_PRESAGE
class CandidatesCallback
    : public PresageCallback
{
private:
    const std::string& m_past_context;
    const std::string m_empty;

public:
    explicit CandidatesCallback(const std::string& past_context);

    std::string get_past_stream() const;
    std::string get_future_stream() const;
};

CandidatesCallback::CandidatesCallback(const std::string &past_context)
    : m_past_context(past_context)
    , m_empty()
{}

std::string CandidatesCallback::get_past_stream() const
{
    return m_past_context;
}

std::string CandidatesCallback::get_future_stream() const
{
    return m_empty;
}
#endif
//! \internal_end

class WordEnginePrivate
{
public:

    enum PredictiveBackend {
        PresageBackend = 1,
        PinyinBackend
    };

    PredictiveBackend predictiveBackend;
    bool use_predictive_text;

    SpellChecker spell_checker;
    bool use_spell_checker;
#ifdef HAVE_PRESAGE
    std::string candidates_context;
    CandidatesCallback presage_candidates;
    Presage presage;
#endif

    // TODO there is too many ifdef in this class
#ifdef HAVE_PINYIN
    PinyinAdapter* pinyinAdapter;
#endif
    explicit WordEnginePrivate();
};

WordEnginePrivate::WordEnginePrivate()
    : predictiveBackend(PresageBackend)
    , use_predictive_text(false)
    , spell_checker()
    , use_spell_checker(false)
#ifdef HAVE_PRESAGE
    , candidates_context()
    , presage_candidates(CandidatesCallback(candidates_context))
    , presage(&presage_candidates)
#endif
{
#ifdef HAVE_PRESAGE
    presage.config("Presage.Selector.SUGGESTIONS", "6");
    presage.config("Presage.Selector.REPEAT_SUGGESTIONS", "yes");
#endif

#ifdef HAVE_PINYIN
    pinyinAdapter = new PinyinAdapter;
#endif
}


//! \brief Constructor.
//! \param parent The owner of this instance. Can be 0, in case QObject
//!               ownership is not required.
WordEngine::WordEngine(QObject *parent)
    : AbstractWordEngine(parent)
    , d_ptr(new WordEnginePrivate)
{}

//! \brief Destructor.
WordEngine::~WordEngine()
{}

//! \brief WordEngine::isEnabled returns if the word engine is functional and enabled
//! \return
bool WordEngine::isEnabled() const
{
    Q_D(const WordEngine);
    return (AbstractWordEngine::isEnabled() &&
            (d->use_predictive_text || d->spell_checker.enabled()));
}

void WordEngine::setWordPredictionEnabled(bool enabled)
{
    Q_D(WordEngine);
 // Don't allow to enable word engine if no backends are available:
#if defined(HAVE_PRESAGE) || defined(HAVE_HUNSPELL) || defined(HAVE_PINYIN)
#else
    if (enabled) {
        qWarning() << __PRETTY_FUNCTION__
                   << "No backend available, cannot enable word engine!";
    }

    enabled = false;
#endif
    if (enabled == d->use_predictive_text)
        return;

    bool totalEnabled = isEnabled();

    d->use_predictive_text = enabled;

    if(totalEnabled != isEnabled())
        Q_EMIT enabledChanged(isEnabled());
}

//! \brief WordEngine::enableSpellcheker turns on/off the usage of the spellchecker
//! \param enabled
void WordEngine::setSpellcheckerEnabled(bool enabled)
{
    Q_D(WordEngine);
    bool totalEnabled = isEnabled();

    d->use_spell_checker = enabled;

    d->spell_checker.setEnabled(d->use_spell_checker);
    if(totalEnabled != isEnabled())
        Q_EMIT enabledChanged(isEnabled());
}

void WordEngine::onWordCandidateSelected(QString word)
{
#ifdef HAVE_PINYIN
    Q_D(WordEngine);
    if (d->predictiveBackend == WordEnginePrivate::PinyinBackend)
        d->pinyinAdapter->wordCandidateSelected(word);
#else
    Q_UNUSED(word);
#endif
}

WordCandidateList WordEngine::fetchCandidates(Model::Text *text)
{
    Q_D(WordEngine);

    WordCandidateList candidates;
    const QString &preedit(text->preedit());
    const bool is_preedit_capitalized(not preedit.isEmpty() && preedit.at(0).isUpper());

    if (d->use_predictive_text) {
#ifdef HAVE_PINYIN
        if (d->predictiveBackend == WordEnginePrivate::PinyinBackend) {
            QString sentence = d->pinyinAdapter->parse(preedit);

            QStringList suggestions = d->pinyinAdapter->getWordCandidates();

            Q_FOREACH(const QString &suggestion, suggestions) {
                appendToCandidates(&candidates, WordCandidate::SourcePrediction, suggestion, is_preedit_capitalized);
            }
        }
#endif

#ifdef HAVE_PRESAGE
        if (d->predictiveBackend == WordEnginePrivate::PresageBackend) {
            const QString &context = (text->surroundingLeft() + preedit);
            d->candidates_context = context.toStdString();
            const std::vector<std::string> predictions = d->presage.predict();

            // TODO: Fine-tune presage behaviour to also perform error correction, not just word prediction.
            if (not context.isEmpty()) {
                // FIXME: max_candidates should come from style, too:
                const static unsigned int max_candidates = 7;
                const int count(qMin<int>(predictions.size(), max_candidates));
                for (int index = 0; index < count; ++index) {
                    appendToCandidates(&candidates, WordCandidate::SourcePrediction, QString::fromStdString(predictions.at(index)),
                                       is_preedit_capitalized);
                }
            }
        }
#endif
    }

    const bool correct_spelling(d->spell_checker.spell(preedit));

    if (candidates.isEmpty() and not correct_spelling) {
        Q_FOREACH(const QString &correction, d->spell_checker.suggest(preedit, 5)) {
            appendToCandidates(&candidates, WordCandidate::SourceSpellChecking, correction, is_preedit_capitalized);
        }
    }

    text->setPreeditFace(candidates.isEmpty() ? (correct_spelling ? Model::Text::PreeditDefault
                                                                  : Model::Text::PreeditNoCandidates)
                                              : Model::Text::PreeditActive);

    text->setPrimaryCandidate(candidates.isEmpty() ? QString()
                                                   : candidates.first().label());


    return candidates;
}

void WordEngine::addToUserDictionary(const QString &word)
{
    Q_D(WordEngine);
    d->spell_checker.addToUserWordlist(word);
}

void WordEngine::onLanguageChanged(const QString &languageId)
{
    Q_D(WordEngine);

    if (languageId == "zh")
        d->predictiveBackend = WordEnginePrivate::PinyinBackend;
    else
        d->predictiveBackend = WordEnginePrivate::PresageBackend;

    bool ok = d->spell_checker.setLanguage(languageId);
    if (ok)
        d->spell_checker.setEnabled(d->use_spell_checker);
}

}} // namespace Logic, MaliitKeyboard
