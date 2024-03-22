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

#ifndef MALIIT_KEYBOARD_ABSTRACTWORDENGINE_H
#define MALIIT_KEYBOARD_ABSTRACTWORDENGINE_H

#include "models/text.h"
#include "models/wordcandidate.h"

class AbstractLanguageFeatures;

namespace MaliitKeyboard {
namespace Logic {

class AbstractWordEnginePrivate;

class AbstractWordEngine
    : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractWordEngine)
    Q_DECLARE_PRIVATE(AbstractWordEngine)

    Q_PROPERTY(bool enabled READ isEnabled
                            WRITE setEnabled
                            NOTIFY enabledChanged)

public:
    explicit AbstractWordEngine(QObject *parent = 0);
    virtual ~AbstractWordEngine();

    virtual bool isEnabled() const;
    Q_SLOT virtual void setEnabled(bool enabled);
    Q_SIGNAL void enabledChanged(bool enabled);

    Q_SLOT virtual void setWordPredictionEnabled(bool on);
    Q_SLOT virtual void setSpellcheckerEnabled(bool on);
    Q_SLOT virtual void setAutoCorrectEnabled(bool on);

    virtual void clearCandidates();
    void computeCandidates(Model::Text *text);
    Q_SIGNAL void candidatesChanged(const WordCandidateList &candidates);

    virtual void addToUserDictionary(const QString &word);

    virtual AbstractLanguageFeatures* languageFeature() = 0;

public Q_SLOTS:
    virtual void onWordCandidateSelected(QString word) = 0;
    virtual void onLanguageChanged(const QString& pluginPath, const QString& languageId) = 0;
    virtual void updateQmlCandidates(QStringList qmlCandidates) = 0;

signals:
    void preeditFaceChanged(Model::Text::PreeditFace face);
    void primaryCandidateChanged(QString candidate);
    void pluginChanged();
    void commitTextRequested(const QString &text);

private:
    virtual void fetchCandidates(Model::Text *text) = 0;
    const QScopedPointer<AbstractWordEnginePrivate> d_ptr;
};

}} // namespace MaliitKeyboard, Logic

#endif // MALIIT_KEYBOARD_ABSTRACTWORDENGINE_H
