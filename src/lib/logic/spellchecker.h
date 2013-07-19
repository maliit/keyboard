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

#ifndef MALIIT_KEYBOARD_SPELLCHECKER_H
#define MALIIT_KEYBOARD_SPELLCHECKER_H

#include <QtCore>

namespace MaliitKeyboard {
namespace Logic {

class SpellCheckerPrivate;

class SpellChecker
{
    Q_DISABLE_COPY(SpellChecker)
    Q_DECLARE_PRIVATE(SpellChecker)
public:
    // FIXME: Find better way to discover default dictionaries.
    // FIXME: Allow changing languages in between.
    explicit SpellChecker(const QString &dictionary_path = QString("%1/en_GB").arg(SpellChecker::dictPath()),
                          const QString &user_dictionary = QString("%1/.config/maliit/userwords.txt").arg(QDir::homePath()));

    ~SpellChecker();

    bool spell(const QString &word);
    QStringList suggest(const QString &word,
                        int limit = -1);
    void ignoreWord(const QString &word);
    void addToUserWordlist(const QString &word);

    static QString dictPath();

private:
    const QScopedPointer<SpellCheckerPrivate> d_ptr;
};

}} // namespace Logic, MaliitKeyboard

#endif // MALIIT_KEYBOARD_SPELLCHECKER_H
