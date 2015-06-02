#ifndef LANGUAGEPLUGININTERFACE_H
#define LANGUAGEPLUGININTERFACE_H

#include <QString>
#include <QStringList>

class AbstractLanguageFeatures;

class LanguagePluginInterface
{
public:
    virtual ~LanguagePluginInterface() {}

    virtual void predict(const QString& surroundingLeft, const QString& preedit) = 0;
    virtual void wordCandidateSelected(QString word) = 0;

    virtual AbstractLanguageFeatures* languageFeature() = 0;

    //! spell checker
    virtual void spellCheckerSuggest(const QString& word, int limit) = 0;
    virtual void addToSpellCheckerUserWordList(const QString& word) = 0;
    virtual bool setLanguage(const QString& languageId, const QString &pluginPath) = 0;
};

#define LanguagePluginInterface_iid "com.canonical.UbuntuKeyboard.LanguagePluginInterface"

Q_DECLARE_INTERFACE(LanguagePluginInterface, LanguagePluginInterface_iid)

#endif // LANGUAGEPLUGININTERFACE_H

