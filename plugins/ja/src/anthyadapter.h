/*
 * Copyright 2015 Canonical Ltd.
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

#ifndef ANTHYADAPTER_H
#define ANTHYADAPTER_H

#include <QObject>
#include <QStringList>

#include "anthy/anthy.h"

class AnthyAdapter : public QObject
{
    Q_OBJECT

public:
    explicit AnthyAdapter(QObject *parent = nullptr);
    ~AnthyAdapter() override;

    QStringList candidates;

signals:
    void newPredictionSuggestions(QString, QStringList);

public slots:
    void parse(const QString& string);
    void wordCandidateSelected(const QString& word);

private:
    anthy_context_t  m_context;
};
#endif // ANTHYADAPTER_H
