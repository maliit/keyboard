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

#include "abstractwordengine.h"

namespace MaliitKeyboard {
namespace Logic {

//! \class AbstractWordEngine
//! \brief Provides word candidates based on text model.
//!
//! Derived classes need to provide an implementation for
//! fetchCandidates() and, optionally, addToUserDictionary().
//! \sa Model::Text, computeCandidates().

//! \fn void AbstractWordEngine::enabledChanged(bool enabled)
//! \brief Emitted when word engine toggles word candidate updates on/off.
//! \param enabled Whether word engine is enabled.

//! \fn void AbstractWordEngine::candidatesChanged(const WordCandidateList &candidates)
//! \brief Emitted when new candidates have been computed.
//! \param candidates The list of updated candidates.

//! \fn WordCandidateList AbstractWordEngine::fetchCandidates(Model::Text *text)
//! \brief Returns a list of candidates.
//! \param text The text model.
//!
//! Needs to be implemented by derived classes. Will not be called if engine
//! is disabled or text model has no preedit.

//! \property AbstractWordEngine::enabled
//! \brief Whether the engine provides updates for word candidates.

class AbstractWordEnginePrivate
{
public:
    bool enabled;

    explicit AbstractWordEnginePrivate();
};

AbstractWordEnginePrivate::AbstractWordEnginePrivate()
    : enabled(false)
{}


//! \brief Constructor.
//! \param parent The owner of this instance. Can be 0, in case QObject
//!               ownership is not required.
AbstractWordEngine::AbstractWordEngine(QObject *parent)
    : QObject(parent)
    , d_ptr(new AbstractWordEnginePrivate)
{}

//! \brief Destructor.
//!
//! Needs to be implemented in derived classes.
AbstractWordEngine::~AbstractWordEngine()
{}


//! \brief Returns whether the word engine is enabled.
//! \sa AbstractWordEngine::enabled
bool AbstractWordEngine::isEnabled() const
{
    Q_D(const AbstractWordEngine);
    return d->enabled;
}


//! \brief Set whether the engine should be enabled.
//! \param enabled Setting to true will be ignored if there's no word
//!                prediction or error correction backend available.
//! \sa AbstractWordEngine::enabled
void AbstractWordEngine::setEnabled(bool enabled)
{
    Q_D(AbstractWordEngine);

    if (d->enabled != enabled) {
        clearCandidates();
        d->enabled = enabled;
        Q_EMIT enabledChanged(d->enabled);
    }
}


//! \brief Clears the current candidates.
//!
//! Only has an effect when word engine is enabled, in which case
//! candidatesCanged() is emitted.
void AbstractWordEngine::clearCandidates()
{
    if (isEnabled()) {
        Q_EMIT candidatesChanged(WordCandidateList());
    }
}


//! \brief Computes new candidates, based on text model.
//! \param text The text model.
//!
//! Can trigger emission of candidatesChanged().
void AbstractWordEngine::computeCandidates(Model::Text *text)
{
    // FIXME: add possiblity to turn off the error correction for
    // entries that does not need it (like password entries).  Also,
    // with that we probably will want to turn off preedit styling at
    // all.

    if (not isEnabled()
        || not text
        || text->preedit().isEmpty()
        || not text->preedit().at(text->preedit().length() - 1).isLetterOrNumber()) {
        // FIXME: We should here set some special preedit face in text
        // model (say Disabled or None) which would be interpreted by
        // editor to send no formatting informations along with
        // preedit string. When this is done, preedit-string test
        // needs to be adapted.
        return;
    }

    Q_EMIT candidatesChanged(fetchCandidates(text));
}

//! \brief Adds a word to user dictionary.
//! \param word A word.
//!
//! Needs to be implemented in derived classes. This does nothing.
void AbstractWordEngine::addToUserDictionary(const QString &word)
{
    Q_UNUSED(word);
}

}} // namespace MaliitKeyboard, Logic
