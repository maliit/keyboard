#ifndef KOREANPLUGIN_H
#define KOREANPLUGIN_H

#include <QObject>
#include <QStringList>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"
#include "spellchecker.h"
#include "spellpredictworker.h"

#include <iostream>

class KoreanLanguageFeatures;

class KoreanPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "koreanplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit KoreanPlugin(QObject *parent = nullptr);
    ~KoreanPlugin() override;

    void predict(const QString& surroundingLeft, const QString& preedit) override;
    void wordCandidateSelected(QString word) override;
    AbstractLanguageFeatures* languageFeature() override;

    //! spell checker
    void spellCheckerSuggest(const QString& word, int limit) override;
    void addToSpellCheckerUserWordList(const QString& word) override;
    bool setLanguage(const QString& languageId, const QString& pluginPath) override;
    virtual void addSpellingOverride(const QString& orig, const QString& overridden);
    virtual void loadOverrides(const QString& pluginPath);

signals:
    void newSpellCheckWord(QString word);
    void setSpellCheckLimit(int limit);
    void setSpellPredictLanguage(QString language, QString pluginPath);
    void parsePredictionText(QString surroundingLeft, QString preedit);
    void setPredictionLanguage(QString language);
    void addToUserWordList(const QString& word);
    void addOverride(const QString& orig, const QString& overridden);

public slots:
    void spellCheckFinishedProcessing(QString word, QStringList suggestions);

private:
    KoreanLanguageFeatures* m_koreanLanguageFeatures;
    SpellPredictWorker *m_spellPredictWorker;
    QThread *m_spellPredictThread;
    bool m_spellCheckEnabled;
    QString m_nextSpellWord;
    bool m_processingSpelling;
};

#endif // KOREANPLUGIN_H
