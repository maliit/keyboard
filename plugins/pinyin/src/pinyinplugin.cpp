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

    connect(m_pinyinAdapter, SIGNAL(newPredictionSuggestions(QString, QStringList)), this, SLOT(finishedProcessing(QString, QStringList)));
    connect(this, SIGNAL(parsePredictionText(QString)), m_pinyinAdapter, SLOT(parse(QString)));
    connect(this, SIGNAL(candidateSelected(QString)), m_pinyinAdapter, SLOT(wordCandidateSelected(QString)));
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
    Q_EMIT candidateSelected(word);
}

AbstractLanguageFeatures* PinyinPlugin::languageFeature()
{
    return m_chineseLanguageFeatures;
}

void PinyinPlugin::finishedProcessing(QString word, QStringList suggestions)
{
    Q_EMIT newPredictionSuggestions(word, suggestions);
    if (word != m_nextWord) {
        Q_EMIT(parsePredictionText(word));
    } else {
        m_processingWord = false;
    }
}
