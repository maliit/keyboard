#ifndef EXAMPLEPLUGIN_H
#define EXAMPLEPLUGIN_H

#include <QObject>
#include "../src/lib/logic/languageplugininterface.h"

#include "pinyinadapter.h"
#include <iostream>


class ExamplePlugin : public QObject, LanguagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EchoInterface" FILE "exampleplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit ExamplePlugin(QObject *parent = 0);
    virtual ~ExamplePlugin();
    
    QString parse(const QString& str);
    QStringList getWordCandidates();
    void wordCandidateSelected(QString word);

signals:
    
public slots:
    
private:
    PinyinAdapter* pinyinAdapter;
};

#endif // EXAMPLEPLUGIN_H
