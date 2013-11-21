#ifndef PINYINPLUGIN_H
#define PINYINPLUGIN_H

#include <QObject>
#include "../src/lib/logic/languageplugininterface.h"

#include "pinyinadapter.h"
#include <iostream>

class ChineseLanguageFeatures;

class PinyinPlugin : public QObject, LanguagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PinyinPlugin" FILE "pinyinplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit PinyinPlugin(QObject *parent = 0);
    virtual ~PinyinPlugin();
    
    QString parse(const QString& str);
    QStringList getWordCandidates();
    void wordCandidateSelected(QString word);

    virtual AbstractLanguageFeatures* languageFeature();

signals:
    
public slots:
    
private:
    PinyinAdapter* pinyinAdapter;
    ChineseLanguageFeatures* m_chineseLanguageFeatures;
};

#endif // PINYINPLUGIN_H
