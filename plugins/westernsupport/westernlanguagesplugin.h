#ifndef WESTERNLANGUAGESPLUGIN_H
#define WESTERNLANGUAGESPLUGIN_H

#include <QObject>

#include "languageplugininterface.h"
#include "westernlanguagefeatures.h"
#include "spellchecker.h"
#include "abstractlanguageplugin.h"

#include "spellpredictworker.h"

class WesternLanguageFeatures;
class SpellPredictWorker;

class WesternLanguagesPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit WesternLanguagesPlugin(QObject *parent = nullptr);
    ~WesternLanguagesPlugin() override;

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
    WesternLanguageFeatures* m_languageFeatures;
    SpellPredictWorker *m_spellPredictWorker;
    QThread *m_spellPredictThread;
    bool m_spellCheckEnabled;
    QString m_nextSpellWord;
    bool m_processingSpelling;
};

#endif // WESTERNLANGUAGESPLUGIN_H
