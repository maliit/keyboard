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

QString PinyinPlugin::parse(const QString& str)
{
    return pinyinAdapter->parse(str);
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
