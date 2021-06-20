#include "pinyinplugin.h"
#include "chineselanguagefeatures.h"

#include <QDebug>

PinyinPlugin::PinyinPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_chineseLanguageFeatures(new ChineseLanguageFeatures)
  , m_processingWord(false)
{
    m_pinyinThread = new QThread();
    m_pinyinAdapter = new PinyinAdapter();
    m_pinyinAdapter->moveToThread(m_pinyinThread);

    connect(m_pinyinAdapter, &PinyinAdapter::newPredictionSuggestions, this, &PinyinPlugin::finishedProcessing);
    connect(this, &PinyinPlugin::parsePredictionText, m_pinyinAdapter, &PinyinAdapter::parse);
    connect(this, &PinyinPlugin::candidateSelected, m_pinyinAdapter, &PinyinAdapter::wordCandidateSelected);
    connect(m_pinyinAdapter, &PinyinAdapter::completed, this, &AbstractLanguagePlugin::commitTextRequested);
    m_pinyinThread->start();
}

PinyinPlugin::~PinyinPlugin()
{
    m_pinyinAdapter->deleteLater();
    m_pinyinThread->quit();
    m_pinyinThread->wait();
}

void PinyinPlugin::predict(const QString& surroundingLeft, const QString& preedit)
{
    Q_UNUSED(surroundingLeft);
    m_nextWord = preedit;
    if (!m_processingWord) {
        m_processingWord = true;
        Q_EMIT parsePredictionText(preedit);
    }
}

void PinyinPlugin::wordCandidateSelected(QString word)
{
    qDebug() << "Pinyin plugin: selecting word " << word;
    Q_EMIT candidateSelected(word);
}

AbstractLanguageFeatures* PinyinPlugin::languageFeature()
{
    return m_chineseLanguageFeatures;
}

void PinyinPlugin::finishedProcessing(QString word, QStringList suggestions, int strategy)
{
    Q_EMIT newPredictionSuggestions(word, suggestions, strategy);
    if (word != m_nextWord) {
        Q_EMIT parsePredictionText(word);
    } else {
        m_processingWord = false;
    }
}
