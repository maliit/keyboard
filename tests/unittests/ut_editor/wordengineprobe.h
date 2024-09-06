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

#ifndef MALIIT_KEYBOARD_WORDENGINEPROBE_H
#define MALIIT_KEYBOARD_WORDENGINEPROBE_H

#include "logic/abstractwordengine.h"
#include "logic/abstractlanguagefeatures.h"


class MockLanguageFeatures : public AbstractLanguageFeatures
{
public:
    explicit MockLanguageFeatures() {}
    virtual ~MockLanguageFeatures() {}

    virtual bool isSeparator(const QString &text) const;
    virtual bool isSymbol(const QString &text) const;
    virtual bool alwaysShowSuggestions() const { return false; }
    virtual bool autoCapsAvailable() const { return true; }
    virtual bool activateAutoCaps(const QString &preedit) const;
    virtual QString appendixForReplacedPreedit(const QString &preedit) const { Q_UNUSED(preedit); return " "; }
    virtual QString fullStopSequence() const { return QString("."); }
};

namespace MaliitKeyboard {
namespace Logic {

class WordEngineProbe
    : public AbstractWordEngine
{
    Q_OBJECT
    Q_DISABLE_COPY(WordEngineProbe)

public:
    explicit WordEngineProbe(QObject *parent = 0);
    virtual ~WordEngineProbe();

    void addSpellingCandidate(const QString &text, const QString &word);

    virtual AbstractLanguageFeatures* languageFeature();

    Q_SLOT void onWordCandidateSelected(QString) override {};
    Q_SLOT void onLanguageChanged(const QString&, const QString&) override {};

    Q_SLOT void updateQmlCandidates(QStringList) override {};

private:
    virtual void fetchCandidates(Model::Text *text);

    QHash<QString, QString> candidates;
};

}} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_WORDENGINEPROBE_H
