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
#include "abstractlanguageplugin.h"

namespace MaliitKeyboard {
namespace Logic {

#define DEFAULT_PLUGIN MALIIT_KEYBOARD_LANGUAGES_DIR "/en/libenplugin.so"

//! \class WordEngine
//! \brief Provides error correction (based on Hunspell)

class WordEnginePrivate
{
public:

    bool use_predictive_text;
    bool requested_prediction_state; // The state requested by settings prior
                                     // to being overridden by any language
                                     // specific requirements

    bool use_spell_checker;

    bool is_preedit_capitalized;

    bool auto_correct_enabled;

    bool calculated_primary_candidate;

    bool clear_candidates_on_incoming;

    LanguagePluginInterface* languagePlugin;

    QPluginLoader pluginLoader;

    WordCandidateList* candidates;

    Model::Text *currentText;

    explicit WordEnginePrivate();

    QString currentPlugin;
    void loadPlugin(QString pluginPath)
    {
        if (pluginPath == currentPlugin)
            return;

        delete languagePlugin;
        pluginLoader.unload();

        // to avoid hickups in libpinyin
        QLocale::setDefault(QLocale::c());
        setlocale(LC_NUMERIC, "C");

        if (pluginPath == DEFAULT_PLUGIN) {
            QString prefix = qgetenv("KEYBOARD_PREFIX_PATH");
            if (!prefix.isEmpty()) {
                pluginPath = prefix + QDir::separator() + pluginPath;
            }
        }

        pluginLoader.setFileName(pluginPath);
        QObject *plugin = pluginLoader.instance();

        if (plugin) {
            languagePlugin = qobject_cast<LanguagePluginInterface *>(plugin);
            if (!languagePlugin) {
                qCritical() << "wordengine.cpp - loading plugin failed: " + pluginPath;

                // fallback
                if (pluginPath != DEFAULT_PLUGIN)
                    loadPlugin(DEFAULT_PLUGIN);
            } else {
                qDebug() << "wordengine.cpp plugin" << pluginPath << "loaded";
                currentPlugin = pluginPath;
            }
        } else {
            qCritical() << __PRETTY_FUNCTION__ << " Loading plugin failed: " << pluginLoader.errorString();
            // fallback
            if (pluginPath != DEFAULT_PLUGIN)
                loadPlugin(DEFAULT_PLUGIN);
        }
    }
};

WordEnginePrivate::WordEnginePrivate()
    : use_predictive_text(false)
    , requested_prediction_state(false)
    , use_spell_checker(false)
    , is_preedit_capitalized(false)
    , auto_correct_enabled(false)
    , calculated_primary_candidate(false)
    , clear_candidates_on_incoming(false)
    , languagePlugin(nullptr)
    , currentText(nullptr)
{
    loadPlugin(DEFAULT_PLUGIN);
    candidates = new WordCandidateList();
}


//! \brief Constructor.
//! \param parent The owner of this instance. Can be 0, in case QObject
//!               ownership is not required.
WordEngine::WordEngine(QObject *parent)
    : AbstractWordEngine(parent)
    , d_ptr(new WordEnginePrivate)
{
    Q_EMIT preeditFaceChanged(Model::Text::PreeditDefault);
}

//! \brief Destructor.
WordEngine::~WordEngine() = default;

//! \brief WordEngine::isEnabled returns if the word engine is functional and enabled
//! \return
bool WordEngine::isEnabled() const
{
    Q_D(const WordEngine);
    return (AbstractWordEngine::isEnabled() &&
            (d->use_predictive_text || d->use_spell_checker) &&
            d->languagePlugin->languageFeature()->wordEngineAvailable());
}

void WordEngine::appendToCandidates(WordCandidateList *candidates,
                                    WordCandidate::Source source,
                                    const QString &candidate)
{
    Q_D(WordEngine);

    if (not candidates) {
        return;
    }

    QString changed_candidate(candidate);

    if (not changed_candidate.isEmpty() && d->is_preedit_capitalized) {
        changed_candidate[0] = changed_candidate.at(0).toUpper();
    }

    WordCandidate word_candidate(source, changed_candidate);

    if (not candidates->contains(word_candidate)) {
        candidates->append(word_candidate);
    }
}

void WordEngine::setWordPredictionEnabled(bool enabled)
{
    Q_D(WordEngine);

    d->requested_prediction_state = enabled;

    // Don't allow to enable word engine if no backends are available:
    if (!d->languagePlugin && enabled) {
        qWarning() << __PRETTY_FUNCTION__
                   << "No backend available, cannot enable word engine!";
        enabled = false;
    }

    if (d->languagePlugin && d->languagePlugin->languageFeature()->alwaysShowSuggestions()) {
        // Override requested setting for languages that should always
        // display suggestions, such as Pinyin
        enabled = true;
    }

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

    if(totalEnabled != isEnabled())
        Q_EMIT enabledChanged(isEnabled());
}

void WordEngine::setAutoCorrectEnabled(bool enabled)
{
    Q_D(WordEngine);

    d->auto_correct_enabled = enabled;
}

void WordEngine::onWordCandidateSelected(QString word)
{
    Q_D(WordEngine);

    d->languagePlugin->wordCandidateSelected(word);
}

void WordEngine::updateQmlCandidates(QStringList qmlCandidates)
{
    WordCandidateList candidates;
    Q_FOREACH(const QString &qmlCandidate, qmlCandidates) {
        appendToCandidates(&candidates, WordCandidate::SourcePrediction, qmlCandidate);
    }
    Q_EMIT candidatesChanged(candidates);
}

void WordEngine::fetchCandidates(Model::Text *text)
{
    Q_D(WordEngine);

    d->calculated_primary_candidate = false;

    // Allow the current candidates to remain on the word ribbon until
    // a new set have been calculated.
    d->clear_candidates_on_incoming = true;

    d->currentText = text;

    const QString &preedit(text->preedit());
    d->is_preedit_capitalized = not preedit.isEmpty() && preedit.at(0).isUpper();

    Q_EMIT candidatesChanged(*d->candidates);

    Q_EMIT primaryCandidateChanged(QString());

    if (d->use_predictive_text) {
        d->languagePlugin->predict(text->surroundingLeft(), preedit);
    }

    if (d->use_spell_checker) {
        d->languagePlugin->spellCheckerSuggest(preedit, 5);
    }
}

void WordEngine::newSpellingSuggestions(QString word, QStringList suggestions, int strategy)
{
    Q_D(WordEngine);

    if (d->currentText && word != d->currentText->preedit()) {
        // Don't add suggestions coming in for a previous word
        return;
    }

    // Spelling and prediction suggestions arrive asynchronously
    // So we need to ensure only one primary candidate is selected
    suggestionMutex.lock();

    if (d->clear_candidates_on_incoming) {
        clearCandidates();
        d->clear_candidates_on_incoming = false;
    } else if (strategy == UpdateCandidateListStrategy::AlwaysClear) {
        clearCandidates();
    }

    Q_FOREACH(const QString &correction, suggestions) {
        appendToCandidates(d->candidates, WordCandidate::SourceSpellChecking, correction);
    }

    calculatePrimaryCandidate();

    Q_EMIT candidatesChanged(*d->candidates);

    suggestionMutex.unlock();
}

void WordEngine::newPredictionSuggestions(QString word, QStringList suggestions, int strategy)
{
    Q_D(WordEngine);

    if (d->currentText && word != d->currentText->preedit()) {
        // Don't add suggestions coming in for a previous word
        return;
    }

    // Spelling and prediction suggestions arrive asynchronously
    // So we need to ensure only one primary candidate is selected
    suggestionMutex.lock();

    if (d->clear_candidates_on_incoming) {
        clearCandidates();
        d->clear_candidates_on_incoming = false;
    } else if (strategy == UpdateCandidateListStrategy::AlwaysClear) {
        clearCandidates();
    }

    Q_FOREACH(const QString &correction, suggestions) {
        appendToCandidates(d->candidates, WordCandidate::SourcePrediction, correction);
    }

    if (strategy == UpdateCandidateListStrategy::AlwaysClear) {
        forceCalculatePrimaryCandidate();
    } else {
        calculatePrimaryCandidate();
    }

    Q_EMIT candidatesChanged(*d->candidates);

    suggestionMutex.unlock();
}

void WordEngine::calculatePrimaryCandidate()
{
    Q_D(WordEngine);

    if (d->calculated_primary_candidate) {
        // We don't want to evaluate the primary candidate twice per
        // word if we're getting both predictions and spellchecking
        // suggestions
        return;
    }

    return calculatePrimaryCandidateImpl();
}

void WordEngine::forceCalculatePrimaryCandidate()
{
    return calculatePrimaryCandidateImpl();
}

void WordEngine::calculatePrimaryCandidateImpl()
{
    Q_D(WordEngine);

    if (!d->auto_correct_enabled) {
        if (d->candidates->size() > 1 && d->candidates->at(0).word() == d->candidates->at(1).word()) {
            // Avoid duplicating the user input if the first prediction matches
            d->candidates->removeAt(1);
        }
        return;
    }

    auto primaryIndex = d->languagePlugin->languageFeature()->primaryCandidateIndex();

    Q_ASSERT(d->candidates->size() <= 1 || d->candidates->size() > primaryIndex);

    if (d->candidates->size() == 0) {
        // We should always have at least one entry due to the user input
        qWarning() << __PRETTY_FUNCTION__ << "User candidate missing";
    } else if (d->candidates->size() == 1) {
        // We don't have any predictions, so the user input is the primary candidate
        WordCandidate primary = d->candidates->value(0);
        Q_EMIT primaryCandidateChanged(primary.word());
    } else if (d->candidates->at(0).word() == d->candidates->at(primaryIndex).word()) {
        // The user candidate matches the first prediction; remove the prediction
        // and make the user input the primary candidate so as not to duplicate
        // the word.
        d->candidates->removeAt(primaryIndex);
        WordCandidate primary = d->candidates->value(0);
        primary.setPrimary(true);
        d->candidates->replace(0, primary);
        Q_EMIT primaryCandidateChanged(primary.word());
    } else if (d->currentText && d->currentText->restoredPreedit()) {
        // The pre-edit has just been restored by the user pressing backspace after
        // auto-completing a word, so the user input should be the primary candidate
        WordCandidate primary = d->candidates->value(0);
        primary.setPrimary(true);
        d->candidates->replace(0, primary);
        Q_EMIT primaryCandidateChanged(primary.word());
        d->currentText->setRestoredPreedit(false);
    } else if (!d->languagePlugin->languageFeature()->ignoreSimilarity()
               && !similarWords(d->candidates->at(0).word(), d->candidates->at(primaryIndex).word())) {
        // The prediction is too different to the user input, so the user input
        // becomes the primary candidate
        WordCandidate primary = d->candidates->value(0);
        primary.setPrimary(true);
        d->candidates->replace(0, primary);
        Q_EMIT primaryCandidateChanged(primary.word());
    } else {
        // The first prediction is the primary candidate
        WordCandidate primary = d->candidates->value(primaryIndex);
        primary.setPrimary(true);
        d->candidates->replace(primaryIndex, primary);
        Q_EMIT primaryCandidateChanged(primary.word());
    }

    if (d->candidates->size() > 1) {
        d->calculated_primary_candidate = true;
    }

}

void WordEngine::addToUserDictionary(const QString &word)
{
    Q_D(WordEngine);
    d->languagePlugin->addToSpellCheckerUserWordList(word);
}

void WordEngine::onLanguageChanged(const QString &pluginPath, const QString &languageId)
{
    Q_D(WordEngine);

    d->loadPlugin(pluginPath);

    setWordPredictionEnabled(d->requested_prediction_state);

    d->languagePlugin->setLanguage(languageId, QFileInfo(d->currentPlugin).absolutePath());

    Q_EMIT enabledChanged(isEnabled());

    connect(static_cast<AbstractLanguagePlugin *>(d->languagePlugin), &AbstractLanguagePlugin::newSpellingSuggestions,
            this, &WordEngine::newSpellingSuggestions);
    connect(static_cast<AbstractLanguagePlugin *>(d->languagePlugin), &AbstractLanguagePlugin::newPredictionSuggestions,
            this, &WordEngine::newPredictionSuggestions);
    connect(static_cast<AbstractLanguagePlugin *>(d->languagePlugin), &AbstractLanguagePlugin::commitTextRequested,
            this, &WordEngine::commitTextRequested);

    Q_EMIT pluginChanged();
}

AbstractLanguageFeatures* WordEngine::languageFeature()
{
    Q_D(WordEngine);
    return d->languagePlugin->languageFeature();
}

bool WordEngine::similarWords(QString word1, QString word2) {
    // Calculate the Levenshtein distance between the first word and the
    // beginning of the second word. If the distance is too great then word2
    // is not considered to be a suitable prediction for word1.
    word2 = word2.left(word1.size());
    if (word1 == word2) {
        return true;
    }

    int *v0 = (int *) malloc(sizeof(int) * word1.size() + 1);
    int *v1 = (int *) malloc(sizeof(int) * word1.size() + 1);

    for (int i = 0; i < word2.size() + 1; i++) {
        v0[i] = i;
        v1[i] = 0;
    }

    for (int i = 0; i < word1.size(); i++) {
        v1[0] = i + 1;

        for (int j = 0; j < word2.size(); j++) {
            int cost = (word1[i] == word2[i]) ? 0 : 1;
            v1[j + 1] = std::min(v1[j] + 1, v0[j + 1] + 1);
            v1[j + 1] = std::min(v1[j] + 1, v0[j] + cost);
        }

        for (int j = 0; j < word1.size() + 1; j++) {
            v0[j] = v1[j];
        }
    }

    double threshold = std::max(word1.size() / 3.0, 3.0);
    int distance = v1[word2.size()];

    free(v0);
    free(v1);

    return distance <= threshold;
}

void WordEngine::clearCandidates()
{
    Q_D(WordEngine);
    if(isEnabled()) {
        d->candidates = new WordCandidateList();
        if (d->currentText) {
            WordCandidate userCandidate(WordCandidate::SourceUser, d->currentText->preedit());
            d->candidates->append(userCandidate);
        }
        Q_EMIT candidatesChanged(*d->candidates);
    }
}

}} // namespace Logic, MaliitKeyboard
