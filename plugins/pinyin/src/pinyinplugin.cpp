#include "pinyinplugin.h"
#include "chineselanguagefeatures.h"

#include <QDebug>

PinyinPlugin::PinyinPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , pinyinAdapter(new PinyinAdapter)
  , m_chineseLanguageFeatures(new ChineseLanguageFeatures)
{
}

PinyinPlugin::~PinyinPlugin()
{
}

void PinyinPlugin::predict(const QString& surroundingLeft, const QString& preedit)
{
    Q_UNUSED(surroundingLeft);
    pinyinAdapter->parse(preedit);
    Q_EMIT newPredictionSuggestions(preedit, pinyinAdapter->getWordCandidates());
}

void PinyinPlugin::wordCandidateSelected(QString word)
{
    return pinyinAdapter->wordCandidateSelected(word);
}

AbstractLanguageFeatures* PinyinPlugin::languageFeature()
{
    return m_chineseLanguageFeatures;
}
