#ifndef KOREANPLUGIN_H
#define KOREANPLUGIN_H

#include <QObject>
#include <QStringList>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"
#include "candidatescallback.h"
#include "spellchecker.h"
#include "spellpredictworker.h"

#include <iostream>

class KoreanLanguageFeatures;
class CandidatesCallback;

class KoreanPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.KoreanPlugin" FILE "koreanplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit KoreanPlugin(QObject *parent = nullptr);
    virtual ~KoreanPlugin();

    virtual void predict(const QString& surroundingLeft, const QString& preedit);
    virtual void wordCandidateSelected(QString word);
    virtual AbstractLanguageFeatures* languageFeature();

    //! spell checker
    virtual void spellCheckerSuggest(const QString& word, int limit);
    virtual void addToSpellCheckerUserWordList(const QString& word);
    virtual bool setLanguage(const QString& languageId, const QString& pluginPath);
    virtual void addSpellingOverride(const QString& orig, const QString& overriden);
    virtual void loadOverrides(const QString& pluginPath);

signals:
    void newSpellingSuggestions(QString word, QStringList suggestions);
    void newPredictionSuggestions(QString word, QStringList suggestions);
    void newSpellCheckWord(QString word);
    void setSpellCheckLimit(int limit);
    void setSpellPredictLanguage(QString language, QString pluginPath);
    void parsePredictionText(QString surroundingLeft, QString preedit);
    void setPredictionLanguage(QString language);
    void addToUserWordList(const QString& word);
    void addOverride(const QString& orig, const QString& overriden);

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
