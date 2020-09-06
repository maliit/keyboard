#ifndef CHEWINGPLUGIN_H
#define CHEWINGPLUGIN_H

#include <QObject>
#include <QStringList>
#include <QThread>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"

#include "chewingadapter.h"
#include <iostream>

class ChewingLanguageFeatures;

class ChewingPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "chewingplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit ChewingPlugin(QObject *parent = nullptr);
    ~ChewingPlugin() override;
    
    void predict(const QString& surroundingLeft, const QString& preedit) override;
    void wordCandidateSelected(QString word) override;

    AbstractLanguageFeatures* languageFeature() override;

    //! spell checker
    void spellCheckerSuggest(const QString& word, int limit) override { Q_UNUSED(word); Q_UNUSED(limit); }
    void addToSpellCheckerUserWordList(const QString& word) override { Q_UNUSED(word); }
    bool setLanguage(const QString& languageId, const QString& pluginPath) override { Q_UNUSED(languageId); Q_UNUSED(pluginPath); return false; }

signals:
    void parsePredictionText(QString preedit);
    void candidateSelected(QString word);
    
public slots:
    void finishedProcessing(QString word, QStringList suggestions);
    
private:
    QThread *m_chewingThread;
    ChewingAdapter *m_chewingAdapter;
    ChewingLanguageFeatures* m_chewingLanguageFeatures;
    QString m_nextWord;
    bool m_processingWord;
};

#endif // CHEWINGPLUGIN_H
