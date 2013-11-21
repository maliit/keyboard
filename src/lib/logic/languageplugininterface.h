#ifndef LANGUAGEPLUGININTERFACE_H
#define LANGUAGEPLUGININTERFACE_H

#include <QString>
#include <QStringList>

class AbstractLanguageFeatures;

class LanguagePluginInterface
{
public:
    virtual ~LanguagePluginInterface() {}

    virtual QString parse(const QString& str) = 0;
    virtual QStringList getWordCandidates() = 0;
    virtual void wordCandidateSelected(QString word) = 0;

    virtual AbstractLanguageFeatures* languageFeature() = 0;

    //! spell checker
    virtual bool spellCheckerEnabled() = 0;
    virtual bool setSpellCheckerEnabled(bool enabled) = 0;
    virtual bool spell(const QString& word) = 0;
    virtual QStringList suggest(const QString& word, int limit) = 0;
    virtual void addToUserWordList(const QString& word) = 0;
    virtual bool setLanguage(const QString& languageId) = 0;
};

#define LanguagePluginInterface_iid "com.canonical.UbuntuKeyboard.LanguagePluginInterface"

Q_DECLARE_INTERFACE(LanguagePluginInterface, LanguagePluginInterface_iid)

#endif // LANGUAGEPLUGININTERFACE_H

