#ifndef WESTERNLANGUAGESPLUGIN_H
#define WESTERNLANGUAGESPLUGIN_H

#include "languageplugininterface.h"
#include "candidatescallback.h"
#include "westernlanguagefeatures.h"
#include "spellchecker.h"

#include <presage.h>

#include <QObject>

class WesternLanguageFeatures;
class CandidatesCallback;

class WesternLanguagesPlugin : /*public QObject,*/ public LanguagePluginInterface
{
    //Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit WesternLanguagesPlugin(/*QObject *parent = 0*/);
    virtual ~WesternLanguagesPlugin();

    virtual void parse(const QString& surroundingLeft, const QString& preedit);
    virtual QStringList getWordCandidates();
    virtual void wordCandidateSelected(QString word);
    virtual AbstractLanguageFeatures* languageFeature();

    //! spell checker
    virtual bool spellCheckerEnabled();
    virtual bool setSpellCheckerEnabled(bool enabled);
    virtual bool spell(const QString& word);
    virtual QStringList spellCheckerSuggest(const QString& word, int limit);
    virtual void addToSpellCheckerUserWordList(const QString& word);
    virtual bool setSpellCheckerLanguage(const QString& languageId);

signals:

public slots:

protected:
    void _useDatabase(const QString& locale);
private:
    std::string m_candidatesContext;
    CandidatesCallback m_presageCandidates;
    Presage m_presage;
    WesternLanguageFeatures* m_languageFeatures;

    SpellChecker m_spellChecker;
};

#endif // WESTERNLANGUAGESPLUGIN_H
