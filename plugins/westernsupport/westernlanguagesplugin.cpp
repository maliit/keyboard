#include "westernlanguagesplugin.h"
#include "westernlanguagefeatures.h"
#include "spellchecker.h"

#include <QDebug>

WesternLanguagesPlugin::WesternLanguagesPlugin(/*QObject *parent*/) :
//    QObject(parent)
    m_candidatesContext()
  , m_presageCandidates(CandidatesCallback(m_candidatesContext))
  , m_presage(&m_presageCandidates)
  , m_languageFeatures(new WesternLanguageFeatures)
  , m_spellChecker()
{
    m_presage.config("Presage.Selector.SUGGESTIONS", "6");
    m_presage.config("Presage.Selector.REPEAT_SUGGESTIONS", "yes");
}

WesternLanguagesPlugin::~WesternLanguagesPlugin()
{
}

void WesternLanguagesPlugin::parse(const QString& surroundingLeft, const QString& preedit)
{
    m_candidatesContext = (surroundingLeft.toStdString() + preedit.toStdString());
}

QStringList WesternLanguagesPlugin::getWordCandidates()
{
    QStringList list;

    try {
        const std::vector<std::string> predictions = m_presage.predict();

        std::vector<std::string>::const_iterator it;
        for (it = predictions.begin(); it != predictions.end(); ++it) {
            list << QString::fromStdString(*it);
        }

    } catch (int error) {
        qWarning() << "An exception was thrown in libpresage when calling predict(), exception nr: " << error;
    }
    return list;
}

void WesternLanguagesPlugin::wordCandidateSelected(QString word)
{
    Q_UNUSED(word);
}

AbstractLanguageFeatures* WesternLanguagesPlugin::languageFeature()
{
    return m_languageFeatures;
}

bool WesternLanguagesPlugin::spellCheckerEnabled()
{
    return m_spellChecker.enabled();
}

bool WesternLanguagesPlugin::setSpellCheckerEnabled(bool enabled)
{
    return m_spellChecker.setEnabled(enabled);
}

bool WesternLanguagesPlugin::spell(const QString& word)
{
    return m_spellChecker.spell(word);
}

QStringList WesternLanguagesPlugin::spellCheckerSuggest(const QString& word, int limit)
{
    return m_spellChecker.suggest(word, limit);
}

void WesternLanguagesPlugin::addToSpellCheckerUserWordList(const QString& word)
{
    return m_spellChecker.addToUserWordlist(word);
}

bool WesternLanguagesPlugin::setSpellCheckerLanguage(const QString& languageId)
{
    return m_spellChecker.setLanguage(languageId);
}

void WesternLanguagesPlugin::_useDatabase(const QString &locale)
{
    QString dbFileName = "database_"+locale+".db";
    QString fullPath("/usr/share/maliit/plugins/com/ubuntu/lib/"+locale+"/");
    fullPath.append(dbFileName);

    try {
        m_presage.config("Presage.Predictors.DefaultSmoothedNgramPredictor.DBFILENAME", fullPath.toLatin1().data());
    } catch (int error) {
        qWarning() << "An exception was thrown in libpresage when changing language database, exception nr: " << error;
    }
}
