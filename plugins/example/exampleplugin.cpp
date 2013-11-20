#include "exampleplugin.h"

#include <QDebug>


ExamplePlugin::ExamplePlugin(QObject *parent) :
    QObject(parent)
  , pinyinAdapter(new PinyinAdapter)
{
}

ExamplePlugin::~ExamplePlugin()
{
}

QString ExamplePlugin::parse(const QString& str)
{
    return pinyinAdapter->parse(str);
}

QStringList ExamplePlugin::getWordCandidates()
{
    return pinyinAdapter->getWordCandidates();
}

void ExamplePlugin::wordCandidateSelected(QString word)
{
    return pinyinAdapter->wordCandidateSelected(word);
}
