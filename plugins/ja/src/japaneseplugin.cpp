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

    connect(m_anthyAdapter, SIGNAL(newPredictionSuggestions(QString, QStringList)), this, SLOT(finishedProcessing(QString, QStringList)));
    connect(this, SIGNAL(parsePredictionText(QString)), m_anthyAdapter, SLOT(parse(const QString&)));
    connect(this, SIGNAL(candidateSelected(QString)), m_anthyAdapter, SLOT(wordCandidateSelected(const QString&)));

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
        Q_EMIT parsePredictionText(word);
    } else {
        m_processingWord = false;
    }
}

