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

#ifndef MALIIT_KEYBOARD_WORDRIBBON_H
#define MALIIT_KEYBOARD_WORDRIBBON_H

#include "models/wordcandidate.h"
#include "models/area.h"

#include <QtCore>

namespace MaliitKeyboard {

/*
 * WordRibbon serves as a ListModel for QML
 */

// TODO: Create common base class w/ KeyArea
class WordRibbon : public QAbstractListModel
{
    Q_OBJECT
    QVector<WordCandidate> m_candidates;
    QPoint m_origin;
    Area m_area;
    QHash<int, QByteArray> m_roles;
    bool m_enabled;

public:
    explicit WordRibbon(QObject* parent = 0);

    enum WordRibbonRoles {
             WordRole = Qt::UserRole + 1
         };

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const;

    bool valid() const;
    QRect rect() const;

    QPoint origin() const;
    void setOrigin(const QPoint &origin);

    QVector<WordCandidate> candidates() const;
    QVector<WordCandidate> & rCandidates();
    void appendCandidate(const WordCandidate &candidate);
    void clearCandidates();

    Area area() const;
    Area & rArea();
    void setArea(const Area &area);

    Q_PROPERTY(bool enabled
               READ enabled
               WRITE setEnabled
               NOTIFY enabledChanged)
    bool enabled() const;
    void setEnabled(bool enabled);

    //! impl. from LayoutUpdater
    Q_SLOT void onWordCandidatePressed(const WordCandidate &candidate);
    Q_SLOT void onWordCandidateReleased(const WordCandidate &candidate);
    Q_SLOT void onWordCandidatesChanged(const WordCandidateList &candidates);

    Q_SIGNAL void wordCandidateSelected(const QString &candidate);
    Q_SIGNAL void userCandidateSelected(const QString &candidate);

    Q_SLOT void setWordRibbonVisible(bool visible);

Q_SIGNALS:
    void enabledChanged(bool enabled);
};

bool operator==(const WordRibbon &lhs,
                const WordRibbon &rhs);

bool operator!=(const WordRibbon &lhs,
                const WordRibbon &rhs);

} // namespace MaliitKeyboard

#endif // MALIIT_KEYBOARD_WORDRIBBON_H
