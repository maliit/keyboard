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

#include <QDebug>
#include <QCoreApplication>

#define MAX_SUGGESTIONS 100

ChewingAdapter::ChewingAdapter(QObject *parent) :
    QObject(parent),
    m_processingWords(false)
{
}

ChewingAdapter::~ChewingAdapter()
{
}

void ChewingAdapter::parse(const QString& string)
{
    m_candidates.clear();
    Q_EMIT newPredictionSuggestions(string, m_candidates);
}

void ChewingAdapter::wordCandidateSelected(const QString& word)
{
    Q_UNUSED(word)
}

void ChewingAdapter::reset()
{
}

