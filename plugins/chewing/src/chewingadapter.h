/*
 * Copyright 2016 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHEWINGADAPTER_H
#define CHEWINGADAPTER_H

#include <QObject>
#include <QStringList>

#include "chewing.h"

class ChewingAdapter : public QObject
{
    Q_OBJECT

    QStringList m_candidates;
    bool m_processingWords;
    ChewingContext *m_chewingContext;

public:
    explicit ChewingAdapter(QObject *parent = nullptr);
    ~ChewingAdapter() override;

signals:
    void newPredictionSuggestions(QString, QStringList);

public slots:
    void parse(const QString& string);
    void clearChewingPreedit();
    void wordCandidateSelected(const QString& word);
    void reset();
};


#endif // CHEWINGADAPTER_H
