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

#include "pinyinadapter.h"

#include <iostream>

#include <iconv.h>
#include <string>
#include <string.h>

#include <QDebug>
#include <QLoggingCategory>
#include <QCoreApplication>

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
    pinyin_parse_more_full_pinyins(m_instance, string.toLatin1().data());

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

    // HACK TODO remove this and correct behaviour
    m_offset = 0;

    pinyin_guess_candidates(m_instance, m_offset, SORT_BY_PHRASE_LENGTH_AND_PINYIN_LENGTH_AND_FREQUENCY);

    candidates.clear();
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

    Q_EMIT newPredictionSuggestions(string, candidates);
}

void PinyinAdapter::wordCandidateSelected(const QString& word)
{
    auto index = candidates.indexOf(word);
    qCDebug(Pinyin) << "Word chosen is `" << word << "', index=" << index;
    //Q_ASSERT(index != -1);

    lookup_candidate_t * candidate = nullptr;
    if (pinyin_get_candidate(m_instance, index, &candidate)) {
        qCDebug(Pinyin) << "Choosing word, offset was" << m_offset;
        m_offset = pinyin_choose_candidate(m_instance, m_offset, candidate);
        qCDebug(Pinyin) << "Word chosen, offset is now" << m_offset;
    }

    // TODO: Emit this only when the sequence is finished
    Q_EMIT completed(word);
}

void PinyinAdapter::reset()
{
    pinyin_reset(m_instance);
}
