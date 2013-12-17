#include "pinyinplugin.h"
#include "chineselanguagefeatures.h"

#include <QDebug>

PinyinPlugin::PinyinPlugin(QObject *parent) :
    QObject(parent)
  , pinyinAdapter(new PinyinAdapter)
  , m_chineseLanguageFeatures(new ChineseLanguageFeatures)
{
}

PinyinPlugin::~PinyinPlugin()
{
}

void PinyinPlugin::parse(const QString& surroundingLeft, const QString& preedit)
{
    Q_UNUSED(surroundingLeft);
    return pinyinAdapter->parse(preedit);
}

QStringList PinyinPlugin::getWordCandidates()
{
    return pinyinAdapter->getWordCandidates();
}

void PinyinPlugin::wordCandidateSelected(QString word)
{
    return pinyinAdapter->wordCandidateSelected(word);
}

AbstractLanguageFeatures* PinyinPlugin::languageFeature()
{
    return m_chineseLanguageFeatures;
}
