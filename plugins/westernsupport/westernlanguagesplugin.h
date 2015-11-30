#ifndef WESTERNLANGUAGESPLUGIN_H
#define WESTERNLANGUAGESPLUGIN_H

#include <QObject>
#include <presage.h>

#include "languageplugininterface.h"
#include "candidatescallback.h"
#include "westernlanguagefeatures.h"
#include "spellchecker.h"
#include "abstractlanguageplugin.h"
#include "spellpredictworker.h"

class WesternLanguageFeatures;
class CandidatesCallback;

class WesternLanguagesPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit WesternLanguagesPlugin(QObject *parent = 0);
    virtual ~WesternLanguagesPlugin();

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
    WesternLanguageFeatures* m_languageFeatures;
    SpellPredictWorker *m_spellPredictWorker;
    QThread *m_spellPredictThread;
    bool m_spellCheckEnabled;
    QString m_nextSpellWord;
    bool m_processingSpelling;
};

#endif // WESTERNLANGUAGESPLUGIN_H
