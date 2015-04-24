#ifndef PINYINPLUGIN_H
#define PINYINPLUGIN_H

#include <QObject>
#include <QStringList>
#include <QThread>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"

#include "pinyinadapter.h"
#include <iostream>

class ChineseLanguageFeatures;

class PinyinPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PinyinPlugin" FILE "pinyinplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit PinyinPlugin(QObject *parent = 0);
    virtual ~PinyinPlugin();
    
    virtual void predict(const QString& surroundingLeft, const QString& preedit);
    virtual void wordCandidateSelected(QString word);

    virtual AbstractLanguageFeatures* languageFeature();

    //! spell checker
    virtual void spellCheckerSuggest(const QString& word, int limit) { Q_UNUSED(word); Q_UNUSED(limit); }
    virtual void addToSpellCheckerUserWordList(const QString& word) { Q_UNUSED(word); }
    virtual bool setLanguage(const QString& languageId, const QString& pluginPath) { Q_UNUSED(languageId); Q_UNUSED(pluginPath); return false; }

signals:
    void newPredictionSuggestions(QString word, QStringList suggestions);
    void parsePredictionText(QString preedit);
    void candidateSelected(QString word);
    
public slots:
    
private:
    QThread *m_pinyinThread;
    PinyinAdapter *m_pinyinAdapter;
    ChineseLanguageFeatures* m_chineseLanguageFeatures;
};

#endif // PINYINPLUGIN_H
