#ifndef WESTERNPLUGIN_H
#define WESTERNPLUGIN_H

#include <QObject>
#include "../src/lib/logic/languageplugininterface.h"

#include "candidatescallback.h"
#include "spellchecker.h"

#include <presage.h>

class CandidatesCallback;
class LanguageFeatures;

class WesternLanguagePlugin : public QObject, LanguagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.WesternLanguagePlugin" FILE "westernplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit WesternLanguagePlugin(QObject *parent = 0);
    virtual ~WesternLanguagePlugin();

    QString parse(const QString& str);
    QStringList getWordCandidates();
    void wordCandidateSelected(QString word);

    virtual AbstractLanguageFeatures* languageFeature();

    //! spell checker
    virtual bool spellCheckerEnabled();
    virtual bool setSpellCheckerEnabled(bool enabled);
    virtual bool spell(const QString& word);
    virtual QStringList suggest(const QString& word, int limit);
    virtual void addToUserWordList(const QString& word);
    virtual bool setLanguage(const QString& languageId);

signals:
    
public slots:
    
private:
    std::string m_candidatesContext;
    CandidatesCallback m_presageCandidates;
    Presage m_presage;
    LanguageFeatures* m_languageFeatures;

    SpellChecker m_spellChecker;
};

#endif // WESTERNPLUGIN_H
