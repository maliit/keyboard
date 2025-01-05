/*
 * Copyright 2013 Canonical Ltd.
 * Copyright (C) 2021 Tusooa Zhu <tusooa@kazv.moe>
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

#include "pinyinadapter.h"

#include <iostream>
#include <algorithm>
#include <iterator>

#include <iconv.h>
#include <string>
#include <string.h>

#include <QCoreApplication>
#include <QDebug>
#include <QLoggingCategory>

#define MAX_SUGGESTIONS 100

namespace
{
    Q_LOGGING_CATEGORY(Pinyin, "maliit.pinyin")
}

PinyinAdapter::PinyinAdapter(QObject *parent) :
    QObject(parent),
    m_processingWords(false)
{
    m_context = pinyin_init(PINYIN_DATA_DIR, ".");
    m_instance = pinyin_alloc_instance(m_context);

    pinyin_set_options(m_context, IS_PINYIN | PINYIN_INCOMPLETE | USE_DIVIDED_TABLE | USE_RESPLIT_TABLE);
}

PinyinAdapter::~PinyinAdapter()
{
    pinyin_free_instance(m_instance);
    pinyin_fini(m_context);
}

void PinyinAdapter::parse(const QString& string)
{
    m_preedit = string;

    m_currentSequence = getCurrentPinyinSequence(string);

#ifdef PINYIN_DEBUG
    for (int i = 0; i < m_instance->m_pinyin_keys->len; i ++)
    {
        PinyinKey* pykey = &g_array_index(m_instance->m_pinyin_keys, PinyinKey, i);
        gchar* py = pykey->get_pinyin_string();
        std::cout << py << " ";
        g_free(py);
    }
    std::cout << std::endl;
#endif

    genCandidatesForCurrentSequence(string);
}

void PinyinAdapter::wordCandidateSelected(const QString& word)
{
    auto index = candidates.indexOf(word);
    qCDebug(Pinyin) << "Word chosen is `" << word << "', index=" << index;
    if (index == -1 || index == 0) {
        // The user has choosen the preedit text or partially converted sequence
        resetSequence();
        // Special case, we don't need to prepend m_convertedChars
        Q_EMIT completed(word);
        return;
    }

    lookup_candidate_t * candidate = nullptr;
    auto indexInPinyin = index - 1;
    if (pinyin_get_candidate(m_instance, indexInPinyin, &candidate)) {
        qCDebug(Pinyin) << "Choosing word, offset was" << m_offset;
        m_offset = pinyin_choose_candidate(m_instance, m_offset, candidate);
        qCDebug(Pinyin) << "Word chosen, offset is now" << m_offset;
    }

    m_convertedChars += word;

    if (remainingChars().isEmpty()) { // The sequence is completed
        qCDebug(Pinyin) << "Sequence is completed";
        auto textToCommit = m_convertedChars;
        resetSequence();
        Q_EMIT completed(textToCommit);
    } else { // We still have remaining pinyin sequence, re-generate candidate list.
        auto partialResult = m_convertedChars + remainingChars();
        qCDebug(Pinyin) << "Sequence is not completed, refresh candidates";
        qCDebug(Pinyin) << "Partial result is" << partialResult;
        genCandidatesForCurrentSequence(m_preedit, UpdateCandidateListStrategy::AlwaysClear);
    }
}

void PinyinAdapter::reset()
{
    resetSequence();
    pinyin_reset(m_instance);
}

struct PinyinSequenceIterator
{
    using value_type = QString;
    using reference = const QString &;
    using iterator_category = std::input_iterator_tag;
    using pointer = const QString *;
    using difference_type = std::ptrdiff_t;

    pinyin_instance_t *m_instance;
    std::size_t m_offset;
    std::size_t m_next;
    QString m_str;

    PinyinSequenceIterator(pinyin_instance_t *instance, std::size_t offset);

    PinyinSequenceIterator &operator++();
    const QString &operator*() const;

    bool operator==(const PinyinSequenceIterator &that) const;
    bool operator!=(const PinyinSequenceIterator &that) const { return !(*this == that); }
};

PinyinSequenceIterator::PinyinSequenceIterator(pinyin_instance_t *instance, std::size_t offset)
    : m_instance(instance)
    , m_offset(offset)
    , m_next(offset + 1) // to be overridden later
{
    ChewingKey *key;
    auto st = pinyin_get_pinyin_key(m_instance, offset, &key);

    // If it failed, we are maybe at the end of sequence, so just ignore it
    if (!st) { return; }

    char *string{};
    st = pinyin_get_pinyin_string(m_instance, key, &string);
    m_str = QString(string);

    if (!st) { return; }
    g_free(string);

    ChewingKeyRest *keyRest;

    st = pinyin_get_pinyin_key_rest(m_instance, m_offset, &keyRest);
    if (!st) { return; }
    guint16 begin, end;
    st = pinyin_get_pinyin_key_rest_positions(
        m_instance, keyRest, &begin, &end);

    qCDebug(Pinyin) << "begin=" << begin << "end=" << end;
    // the next offset is at the end of the current pinyin
    m_next = end;
}

PinyinSequenceIterator &PinyinSequenceIterator::operator++()
{
    return *this = PinyinSequenceIterator(m_instance, m_next);
}

const QString &PinyinSequenceIterator::operator*() const
{
    return m_str;
}

bool PinyinSequenceIterator::operator==(const PinyinSequenceIterator &that) const
{
    return m_instance == that.m_instance
        && m_offset == that.m_offset;
}

QStringList PinyinAdapter::getCurrentPinyinSequence(const QString &preeditString)
{
    // auto oldSequence = m_currentSequence;

    // auto firstAlphabeticIndex = preeditString.indexOf(QRegExp("[a-z]"));

    // if (firstAlphabeticIndex == -1) {
    //     qCDebug(Pinyin) << "The sequence is completed, ignoring";
    //     return {};
    // }

    // auto chineseCharsCount = firstAlphabeticIndex;

    // m_convertedChars = preeditString.mid(0, chineseCharsCount);

    // auto adjustedString = preeditString.mid(chineseCharsCount);

    resetSequence();

    auto pinyinLength = pinyin_parse_more_full_pinyins(m_instance, preeditString.toUtf8().data());

    if (!pinyinLength) { return {}; }

    QStringList seq;

    std::copy(
        PinyinSequenceIterator(m_instance, 0),
        PinyinSequenceIterator(m_instance, pinyinLength),
        std::back_inserter(seq));

    qCDebug(Pinyin) << "current sequence is" << seq;

    return seq;
}

void PinyinAdapter::resetSequence()
{
    m_offset = 0;
    m_convertedChars.clear();
}

void PinyinAdapter::genCandidatesForCurrentSequence(const QString &string, UpdateCandidateListStrategy strategy)
{
    pinyin_guess_candidates(m_instance, m_offset, SORT_BY_PHRASE_LENGTH_AND_PINYIN_LENGTH_AND_FREQUENCY);

    candidates.clear();

    auto maybePartiallyConvertedSeq = m_convertedChars + remainingChars();

    candidates.append(maybePartiallyConvertedSeq);

    guint len = 0;
    pinyin_get_n_candidate(m_instance, &len);
    len = len > MAX_SUGGESTIONS ? MAX_SUGGESTIONS : len;
    for (unsigned int i = 0 ; i < len; i ++ )
    {
        lookup_candidate_t * candidate = nullptr;

        if (pinyin_get_candidate(m_instance, i, &candidate)) {
            const char* word = nullptr;
            pinyin_get_candidate_string(m_instance, candidate, &word);
            // Translate the token to utf-8 phrase.
            if (word) {
                candidates.append(QString(word));
            }
        }
    }

    qCDebug(Pinyin) << "current string is" << string;
    qCDebug(Pinyin) << "candidates are" << candidates;
    Q_EMIT newPredictionSuggestions(string, candidates, strategy);
}

QStringList PinyinAdapter::remainingSequence() const
{
    return m_currentSequence.mid(m_convertedChars.size());
}

QString PinyinAdapter::remainingChars() const
{
    return m_preedit.mid(m_offset);
}
