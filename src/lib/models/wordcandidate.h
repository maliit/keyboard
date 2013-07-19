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

#ifndef MALIIT_KEYBOARD_WORDCANDIDATE_H
#define MALIIT_KEYBOARD_WORDCANDIDATE_H

#include "models/area.h"
#include "models/label.h"

#include <QtCore>

namespace MaliitKeyboard {

class WordCandidate
{
public:
    enum Source {
        SourceUnknown,
        SourceSpellChecking,
        SourcePrediction,
        SourceUser // Candidate based on current preedit word for adding to the user dictionary
    };

private:
    QPoint m_origin;
    Area m_area;
    Label m_label;
    Source m_source;
    QString m_word;

public:
    explicit WordCandidate();
    WordCandidate(Source source, const QString &word);

    // read-only properties:
    bool valid() const;
    QRect rect() const;

    QPoint origin() const;
    void setOrigin(const QPoint &origin);

    Area area() const;
    Area & rArea();
    void setArea(const Area &area);

    Label label() const;
    Label & rLabel();
    void setLabel(const Label &label);

    Source source() const;
    void setSource(Source source);

    QString word() const;
    void setWord(const QString &word);
};

typedef QList<WordCandidate> WordCandidateList;

bool operator==(const WordCandidate &lhs,
                const WordCandidate &rhs);

bool operator!=(const WordCandidate &lhs,
                const WordCandidate &rhs);

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_WORDCANDIDATE_H
