#include "japaneseplugin.h"
#include "japaneselanguagefeatures.h"

#include <QDebug>

JapanesePlugin::JapanesePlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_japaneseLanguageFeatures(new JapaneseLanguageFeatures)
  , m_processingWord(false)
{
    m_anthyThread = new QThread();
    m_anthyAdapter = new AnthyAdapter();
    m_anthyAdapter->moveToThread(m_anthyThread);

    connect(m_anthyAdapter, &AnthyAdapter::newPredictionSuggestions, this, &JapanesePlugin::finishedProcessing);
    connect(this, &JapanesePlugin::parsePredictionText, m_anthyAdapter, &AnthyAdapter::parse);
    connect(this, &JapanesePlugin::candidateSelected, m_anthyAdapter, &AnthyAdapter::wordCandidateSelected);

    m_anthyThread->start();
}

JapanesePlugin::~JapanesePlugin()
{
    m_anthyAdapter->deleteLater();
    m_anthyThread->quit();
    m_anthyThread->wait();
}

AbstractLanguageFeatures* JapanesePlugin::languageFeature()
{
    return m_japaneseLanguageFeatures;
}

void JapanesePlugin::predict(const QString& surroundingLeft, const QString& preedit)
{
    Q_UNUSED(surroundingLeft)

    m_nextWord = preedit;
    if (!m_processingWord) {
        m_processingWord = true;
        Q_EMIT parsePredictionText(preedit);
    }
}

void JapanesePlugin::wordCandidateSelected(QString word)
{
    Q_EMIT candidateSelected(word);
}

void JapanesePlugin::finishedProcessing(QString word, QStringList suggestions)
{
    Q_EMIT newPredictionSuggestions(word, suggestions);
    if (word != m_nextWord) {
        Q_EMIT parsePredictionText(m_nextWord);
    } else {
        m_processingWord = false;
    }
}

