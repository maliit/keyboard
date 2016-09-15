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

#define CHEWING_MAX_LEN 32

ChewingAdapter::ChewingAdapter(QObject *parent) :
    QObject(parent),
    m_processingWords(false)
{
    m_chewingContext = chewing_new();
    chewing_set_easySymbolInput(m_chewingContext, 0);
    chewing_set_maxChiSymbolLen(m_chewingContext, CHEWING_MAX_LEN);
    chewing_set_spaceAsSelection(m_chewingContext, 0);
}

ChewingAdapter::~ChewingAdapter()
{
    chewing_delete(m_chewingContext);
}

void ChewingAdapter::parse(const QString& string)
{
    m_candidates.clear();
    clearChewingPreedit();

    const QChar *c = string.data();
    while (!c->isNull()) {
        if (c->isSpace()) {
            chewing_handle_Space(m_chewingContext);
        } else {
            chewing_handle_Default(m_chewingContext, c->toLatin1());
        }
        c++;
    }

    char * buf_str = chewing_buffer_String(m_chewingContext);
    QString buffer(buf_str);
    QString choppedBuffer = buffer;
    choppedBuffer.chop(1);
    chewing_free(buf_str);
    
    chewing_cand_open(m_chewingContext);

    if (!chewing_cand_CheckDone(m_chewingContext)) {
        // Get candidate words
        chewing_cand_Enumerate(m_chewingContext);
        while (chewing_cand_hasNext(m_chewingContext)) {
            char *chewingCand = chewing_cand_String(m_chewingContext);
            QString candidate(chewingCand);
            m_candidates.append(choppedBuffer + candidate);
            chewing_free(chewingCand);
        }
    }

    if (chewing_buffer_Len(m_chewingContext) <= chewing_cursor_Current(m_chewingContext)) {    
        // Insert bopomofo string
        m_candidates.prepend(buffer + QString(chewing_bopomofo_String_static(m_chewingContext)));
    }

    chewing_cand_close(m_chewingContext);

    Q_EMIT newPredictionSuggestions(string, m_candidates);
}

void ChewingAdapter::clearChewingPreedit()
{
    int origState = chewing_get_escCleanAllBuf(m_chewingContext);
    // Send a false event, then clean it to wipe the commit
    chewing_handle_Default(m_chewingContext, '1');
    chewing_set_escCleanAllBuf(m_chewingContext, 1);
    chewing_handle_Esc(m_chewingContext);
    chewing_set_escCleanAllBuf(m_chewingContext, origState);
    chewing_clean_preedit_buf(m_chewingContext);
}

void ChewingAdapter::wordCandidateSelected(const QString& word)
{
    Q_UNUSED(word)
}

void ChewingAdapter::reset()
{
    clearChewingPreedit();
}

