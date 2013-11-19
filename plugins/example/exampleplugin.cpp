#include "exampleplugin.h"

#include <QDebug>


ExamplePlugin::ExamplePlugin(QObject *parent) :
    QObject(parent)
  , pinyinAdapter(new PinyinAdapter)
{
    qDebug() << __PRETTY_FUNCTION__;
}

ExamplePlugin::~ExamplePlugin()
{
    qDebug() << __PRETTY_FUNCTION__ << "vdestructor";
}

void ExamplePlugin::hello()
{
    qDebug() << __PRETTY_FUNCTION__ << "<<<<<<<<<<<<<<<<<<<<<<<<<<<";
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
