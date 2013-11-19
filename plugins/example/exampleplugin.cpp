#include "exampleplugin.h"

#include <QDebug>


ExamplePlugin::ExamplePlugin(QObject *parent) :
    QObject(parent)
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
