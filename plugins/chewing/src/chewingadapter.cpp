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

#include "chewingadapter.h"

#include <iostream>

#include <iconv.h>
#include <string>
#include <string.h>

#include <QChar>
#include <QCoreApplication>

ChewingAdapter::ChewingAdapter(QObject *parent) :
    QObject(parent),
    m_processingWords(false)
{
    m_chewingContext = chewing_new();
}

ChewingAdapter::~ChewingAdapter()
{
    chewing_delete(m_chewingContext);
}

void ChewingAdapter::parse(const QString& string)
{
    m_candidates.clear();
    chewing_clean_preedit_buf(m_chewingContext);

    const QChar *c = string.data();
    while (!c->isNull()) {
        chewing_handle_Default(m_chewingContext, c->toLatin1());
        c++;
    }

    chewing_cand_open(m_chewingContext);

    if (!chewing_cand_CheckDone(m_chewingContext)) {
        //get candidate word
        chewing_cand_Enumerate(m_chewingContext);
        while (chewing_cand_hasNext(m_chewingContext)) {
            char *chewingCand = chewing_cand_String(m_chewingContext);
            QString candidate(chewingCand);
            m_candidates.append(candidate);
            chewing_free(chewingCand);
        }
    }

    if (m_candidates.isEmpty()) {
        m_candidates.append(QString(chewing_bopomofo_String_static(m_chewingContext)));
    }

    chewing_cand_close(m_chewingContext);

    Q_EMIT newPredictionSuggestions(string, m_candidates);
}

void ChewingAdapter::wordCandidateSelected(const QString& word)
{
    Q_UNUSED(word)
}

void ChewingAdapter::reset()
{
}

