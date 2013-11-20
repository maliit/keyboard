#include "pinyinplugin.h"

#include <QDebug>


PinyinPlugin::PinyinPlugin(QObject *parent) :
    QObject(parent)
  , pinyinAdapter(new PinyinAdapter)
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
