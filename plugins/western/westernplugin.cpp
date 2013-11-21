#include "westernplugin.h"
#include "languagefeatures.h"
#include "spellchecker.h"

#include <QDebug>

WesternLanguagePlugin::WesternLanguagePlugin(QObject *parent) :
    QObject(parent)
  , m_candidatesContext()
  , m_presageCandidates(CandidatesCallback(m_candidatesContext))
  , m_presage(&m_presageCandidates)
  , m_languageFeatures(new LanguageFeatures)
  , m_spellChecker()
{
    m_presage.config("Presage.Selector.SUGGESTIONS", "6");
    m_presage.config("Presage.Selector.REPEAT_SUGGESTIONS", "yes");
}

WesternLanguagePlugin::~WesternLanguagePlugin()
{
}

// TODO no return
QString WesternLanguagePlugin::parse(const QString& str)
{
    m_candidatesContext = str.toStdString();

    return str;
}

QStringList WesternLanguagePlugin::getWordCandidates()
{
    const std::vector<std::string> predictions = m_presage.predict();

    QStringList list;
    std::vector<std::string>::const_iterator it;
    for (it = predictions.begin(); it != predictions.end(); ++it)
        list << QString::fromStdString(*it);

    return list;
}

void WesternLanguagePlugin::wordCandidateSelected(QString word)
{
    Q_UNUSED(word);
}

AbstractLanguageFeatures* WesternLanguagePlugin::languageFeature()
{
    return m_languageFeatures;
}

bool WesternLanguagePlugin::spellCheckerEnabled()
{
    return m_spellChecker.enabled();
}

bool WesternLanguagePlugin::setSpellCheckerEnabled(bool enabled)
{
    return m_spellChecker.setEnabled(enabled);
}

bool WesternLanguagePlugin::spell(const QString& word)
{
    return m_spellChecker.spell(word);
}

QStringList WesternLanguagePlugin::suggest(const QString& word, int limit)
{
    return m_spellChecker.suggest(word, limit);
}

void WesternLanguagePlugin::addToUserWordList(const QString& word)
{
    return m_spellChecker.addToUserWordlist(word);
}

bool WesternLanguagePlugin::setLanguage(const QString& languageId)
{
    return m_spellChecker.setLanguage(languageId);
}

