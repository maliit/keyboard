/*
 * Copyright (C) 2014 Canonical, Ltd.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * Neither the name of Nokia Corporation nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "predictivetextworker.h"

#include <QDebug>

PredictiveTextWorker::PredictiveTextWorker(QObject *parent)
    : QObject(parent)
    , m_candidatesContext()
    , m_presageCandidates(CandidatesCallback(m_candidatesContext))
    , m_presage(&m_presageCandidates)
{
    m_presage.config("Presage.Selector.SUGGESTIONS", "6");
    m_presage.config("Presage.Selector.REPEAT_SUGGESTIONS", "yes");
}

void PredictiveTextWorker::parsePredictionText(const QString& surroundingLeft, const QString& preedit)
{
    m_candidatesContext = (surroundingLeft.toStdString() + preedit.toStdString());

    QStringList list;

    try {
        const std::vector<std::string> predictions = m_presage.predict();

        std::vector<std::string>::const_iterator it;
        for (it = predictions.begin(); it != predictions.end(); ++it) {
            list << QString::fromStdString(*it);
        }

    } catch (int error) {
        qWarning() << "An exception was thrown in libpresage when calling predict(), exception nr: " << error;
    }

    qDebug() << "Predictive text suggestions: " << list;

    Q_EMIT newSuggestions(list);
}

void PredictiveTextWorker::setPredictionLanguage(QString locale)
{
    QString dbFileName = "database_"+locale+".db";
    QString fullPath("/usr/share/maliit/plugins/com/ubuntu/lib/"+locale+"/");
    fullPath.append(dbFileName);

    try {
        m_presage.config("Presage.Predictors.DefaultSmoothedNgramPredictor.DBFILENAME", fullPath.toLatin1().data());
    } catch (int error) {
        qWarning() << "An exception was thrown in libpresage when changing language database, exception nr: " << error;
    }
}
