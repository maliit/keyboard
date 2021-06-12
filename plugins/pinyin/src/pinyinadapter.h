/*
 * Copyright 2013 Canonical Ltd.
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

#ifndef PINYINADAPTER_H
#define PINYINADAPTER_H

#include <QObject>
#include <QStringList>

#include "pinyin.h"

class PinyinAdapter : public QObject
{
    Q_OBJECT

    QStringList candidates;

    pinyin_context_t*  m_context;
    pinyin_instance_t* m_instance;

    bool m_processingWords;
    std::size_t m_offset{};

public:
    explicit PinyinAdapter(QObject *parent = nullptr);
    ~PinyinAdapter() override;

signals:
    void newPredictionSuggestions(QString, QStringList);
    /*!
     * \brief Signals that the whole Pinyin sequence is converted
     * to Chinese characters.
     *
     * \param text The converted Chinese characters.
     */
    void completed(const QString &text);

public slots:
    void parse(const QString& string);
    void wordCandidateSelected(const QString& word);
    void reset();
};


#endif // PINYINADAPTER_H
