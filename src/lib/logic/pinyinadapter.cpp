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

#ifdef HAVE_PINYIN

#include "pinyinadapter.h"

#include <iostream>

#include <iconv.h>
#include <string>
#include <string.h>

#include <QDebug>

PinyinAdapter::PinyinAdapter(QObject *parent) :
    QObject(parent)
{
    m_context = pinyin_init(PINYIN_DATA_DIR, ".");
    m_instance = pinyin_alloc_instance(m_context);

    pinyin_set_options(m_context, IS_PINYIN | USE_DIVIDED_TABLE | USE_RESPLIT_TABLE);
}

PinyinAdapter::~PinyinAdapter()
{
    g_array_free(m_array, TRUE);
    pinyin_free_instance(m_instance);
    pinyin_fini(m_context);
}

QString PinyinAdapter::parse(const QString& string)
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


    pinyin_guess_candidates(m_instance, 0);

    pinyin_guess_sentence(m_instance);

    char* sentence = NULL;
    pinyin_get_sentence(m_instance, &sentence);

    candidates.clear();
    guint len = 0;
    pinyin_get_n_candidate(m_instance, &len);
    for (unsigned int i = 0 ; i < len; i ++ )
    {
        lookup_candidate_t * candidate = NULL;
	pinyin_get_candidate(m_instance, i, &candidate);

        const char* word = NULL;
	pinyin_get_candidate_string(m_instance, candidate, &word);

        // Translate the token to utf-8 phrase.
        if (word) {
            candidates.append(QString(word));
        }
    }

    QString unicode(sentence);
    g_free(sentence);
    return unicode;
}

QStringList PinyinAdapter::getWordCandidates() const
{
    return candidates;
}

void PinyinAdapter::wordCandidateSelected(const QString& word)
{
    Q_UNUSED(word)

    lookup_candidate_t * candidate = NULL;
    pinyin_get_candidate(m_instance, 1, &candidate);
    pinyin_choose_candidate(m_instance, 0, candidate);
}

void PinyinAdapter::reset()
{
    pinyin_reset(m_instance);
}

#endif
