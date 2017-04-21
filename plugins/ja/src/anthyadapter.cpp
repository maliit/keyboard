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

#include "anthyadapter.h"

#include <QDebug>

#ifdef JA_DEBUG
static void anthy_log(int level, const char *log)
{
    Q_UNUSED(level)

    qWarning() << "[anthy]: " << QString(log);
}
#endif

AnthyAdapter::AnthyAdapter(QObject *parent) :
    QObject(parent)
{
#ifdef JA_DEBUG
    anthy_set_logger(anthy_log, 0);
#endif
    if (anthy_init() < 0)
        qCritical() << "[anthy] failed to init.";

    m_context = anthy_create_context();
    if (m_context == nullptr)
        qCritical() << "[anthy] failed to create anthy context.";

    anthy_context_set_encoding(m_context, ANTHY_UTF8_ENCODING);
}

AnthyAdapter::~AnthyAdapter()
{
    anthy_release_context(m_context);
    anthy_quit();
}

#define CANDIDATE_SIZE 1024
void AnthyAdapter::parse(const QString& string)
{
    struct anthy_conv_stat cs;
    struct anthy_segment_stat ss;
    char buf[CANDIDATE_SIZE];
    QString candidate, trail;

    if (anthy_set_string(m_context, string.toUtf8().constData()) != 0) {
        qCritical() << "[anthy] failed to set string: " << string;
    }

    if (anthy_get_stat(m_context, &cs) != 0) {
        qCritical() << "[anthy] failed to get stat: " << string;
    }

    if (anthy_get_segment_stat(m_context, 0, &ss) != 0) {

        qCritical() << "[anthy] failed to get segment stat: " << string;
    }

    /* Nth segment (N > 0) use only first candidate */
    if (cs.nr_segment > 1) {
        for (int i = 1; i < cs.nr_segment; ++i) {
            if (anthy_get_segment(m_context, i, 0, nullptr, 0) > CANDIDATE_SIZE - 1) {
                qCritical() << "[anthy] buffer overflow: " << string;
                continue;
            }

            if (anthy_get_segment(m_context, i, 0, buf, sizeof(buf)) < 0) {
                qCritical() << "[anthy] failed to get segment: " << string;
                continue;
            }

            trail.append(buf);
        }
    }

    /* Create candidate list for 1st segment */
    candidates.clear();
    candidates.append(string);
    for (int i = 0; i < ss.nr_candidate; ++i) {
        if (anthy_get_segment(m_context, 0, i, nullptr, 0) > CANDIDATE_SIZE - 1) {
            qCritical() << "[anthy] buffer overflow: " << string;
            continue;
        }

        if (anthy_get_segment(m_context, 0, i, buf, sizeof(buf)) < 0) {
            qCritical() << "[anthy] failed to get segment: " << string;
            continue;
        }

        candidate = QString(buf);
        candidate.append(trail);
        candidates.append(candidate);
    }

    Q_EMIT newPredictionSuggestions(string, candidates);
}

void AnthyAdapter::wordCandidateSelected(const QString& word)
{
    Q_UNUSED(word)

    anthy_reset_context(m_context);
}
