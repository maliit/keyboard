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
#include "abstractlanguageplugin.h"

class PinyinAdapter : public QObject
{
    Q_OBJECT

    QStringList candidates;

    pinyin_context_t*  m_context;
    pinyin_instance_t* m_instance;

    bool m_processingWords;
    QStringList m_currentSequence;
    QString m_convertedChars;
    QString m_preedit;
    std::size_t m_offset{};

public:
    explicit PinyinAdapter(QObject *parent = nullptr);
    ~PinyinAdapter() override;

signals:
    void newPredictionSuggestions(QString, QStringList, int strategy = UpdateCandidateListStrategy::ClearWhenNeeded);
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

private:
    /*!
     * \brief Obtain the current pinyin sequence from preedit string.
     */
    QStringList getCurrentPinyinSequence(const QString &preeditString);

    /*!
     * \brief Reset the current pinyin sequence.
     */
    void resetSequence();

    /*!
     * \brief Get the candidates for the current pinyin sequence.
     *
     * This functions emits newPredictionSuggestions().
     */
    void genCandidatesForCurrentSequence(const QString &preedit, UpdateCandidateListStrategy strategy = UpdateCandidateListStrategy::ClearWhenNeeded);

    QStringList remainingSequence() const;
    QString remainingChars() const;
};


#endif // PINYINADAPTER_H
