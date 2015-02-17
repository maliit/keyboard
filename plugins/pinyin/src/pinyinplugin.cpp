#include "pinyinplugin.h"
#include "chineselanguagefeatures.h"

#include <QDebug>

PinyinPlugin::PinyinPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_chineseLanguageFeatures(new ChineseLanguageFeatures)
{
    m_pinyinThread = new QThread();
    PinyinAdapter *pinyinAdapter = new PinyinAdapter();
    pinyinAdapter->moveToThread(m_pinyinThread);

    connect(pinyinAdapter, SIGNAL(newPredictionSuggestions(QString, QStringList)), this, SIGNAL(newPredictionSuggestions(QString, QStringList)));
    connect(this, SIGNAL(parsePredictionText(QString)), pinyinAdapter, SLOT(parse(QString)));
    connect(this, SIGNAL(candidateSelected(QString)), pinyinAdapter, SLOT(wordCandidateSelected(QString)));
    m_pinyinThread->start();
}

PinyinPlugin::~PinyinPlugin()
{
    delete pinyinAdapter;
}

void PinyinPlugin::predict(const QString& surroundingLeft, const QString& preedit)
{
    Q_UNUSED(surroundingLeft);
    Q_EMIT parsePredictionText(preedit);
}

void PinyinPlugin::wordCandidateSelected(QString word)
{
    Q_EMIT candidateSelected(word);
}

AbstractLanguageFeatures* PinyinPlugin::languageFeature()
{
    return m_chineseLanguageFeatures;
}
