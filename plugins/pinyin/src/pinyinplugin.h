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
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "pinyinplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit PinyinPlugin(QObject *parent = nullptr);
    ~PinyinPlugin() override;

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
    void finishedProcessing(QString word, QStringList suggestions, int strategy = UpdateCandidateListStrategy::ClearWhenNeeded);

private:
    QThread *m_pinyinThread;
    PinyinAdapter *m_pinyinAdapter;
    ChineseLanguageFeatures* m_chineseLanguageFeatures;
    QString m_nextWord;
    bool m_processingWord;
};

#endif // PINYINPLUGIN_H
