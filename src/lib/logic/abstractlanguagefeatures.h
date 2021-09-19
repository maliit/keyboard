/*
 * This file is part of Maliit Plugins
 *
 * Copyright (C) 2012 Openismus GmbH
 *
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

#ifndef MALIIT_KEYBOARD_ABSTRACTLANGUAGEFEATURES_H
#define MALIIT_KEYBOARD_ABSTRACTLANGUAGEFEATURES_H

#include <QObject>
#include <maliit/plugins/abstractinputmethod.h>

class QObject;
class AbstractLanguageFeatures
{
    // FIXME: Add a language/locale property (also for AbstractWordEngine)
public:
    virtual ~AbstractLanguageFeatures() {}

    virtual bool alwaysShowSuggestions() const = 0;
    virtual bool autoCapsAvailable() const = 0;
    virtual bool activateAutoCaps(const QString &preedit) const = 0;
    virtual QString appendixForReplacedPreedit(const QString &preedit) const = 0;
    virtual bool isSeparator(const QString &text) const { Q_UNUSED(text); return false; }
    virtual QString fullStopSequence() const { return QString(); }
    virtual bool isSymbol(const QString &text) const { Q_UNUSED(text); return false; }
    // Typically we disable auto-correct if the predicted word isn't similar
    // to the user's input. However for input methods such as pinyin this
    // can be disabled by implementing this method to return true.
    virtual bool ignoreSimilarity() const { return false; }
    virtual bool wordEngineAvailable() const { return false; }
    virtual bool enablePreeditAtInsertion() const { return false; }
    virtual bool restorePreedit() const { return true; }
    virtual bool commitOnSpace() const { return true; }
    virtual bool showPrimaryInPreedit() const { return false; }

    /*!
     * \brief Whether we should delay committing the candidate word selected.
     *
     * In some input methods (e.g. Chinese Pinyin), there may exist partial
     * candidates. That is, a candidate word may correspond to a part of
     * the preedit string. In this case, when a user select a candidate word,
     * we should refrain from committing *that* word directly, but wait for
     * the input method to request committing.
     *
     * \returns true if we should delay committing the selected candidate,
     * false otherwise.
     */
    virtual bool shouldDelayCandidateCommit() const { return false; }

    /*!
     * The index of primary candidate in the list.
     *
     * Note that the candidate list signalled by the corresponding language
     * plugin must be of either size 0 or 1, or a size greater than or
     * equal to primaryCandidateIndex()
     */
    virtual std::size_t primaryCandidateIndex() const { return 1; }

    Maliit::TextContentType contentType() const { return m_contentType; }
    void setContentType(Maliit::TextContentType contentType) { m_contentType = contentType; }

private:
    Maliit::TextContentType m_contentType;
};

#endif // MALIIT_KEYBOARD_ABSTRACTLANGUAGEFEATURES_H
