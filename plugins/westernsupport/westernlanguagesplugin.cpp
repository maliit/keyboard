#include "westernlanguagesplugin.h"
#include "westernlanguagefeatures.h"
#include "spellpredictworker.h"

#include <QDebug>

WesternLanguagesPlugin::WesternLanguagesPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_languageFeatures(new WesternLanguageFeatures)
  , m_spellCheckEnabled(false)
{
    m_spellPredictThread = new QThread();
    m_spellPredictWorker = new SpellPredictWorker();
    m_spellPredictWorker->moveToThread(m_spellPredictThread);

    connect(m_spellPredictWorker, SIGNAL(newSpellingSuggestions(QString, QStringList)), this, SIGNAL(newSpellingSuggestions(QString, QStringList)));
    connect(m_spellPredictWorker, SIGNAL(newPredictionSuggestions(QString, QStringList)), this, SIGNAL(newPredictionSuggestions(QString, QStringList)));
    connect(this, SIGNAL(newSpellCheckWord(QString)), m_spellPredictWorker, SLOT(newSpellCheckWord(QString)));
    connect(this, SIGNAL(setSpellPredictLanguage(QString, QString)), m_spellPredictWorker, SLOT(setLanguage(QString, QString)));
    connect(this, SIGNAL(setSpellCheckLimit(int)), m_spellPredictWorker, SLOT(setSpellCheckLimit(int)));
    connect(this, SIGNAL(parsePredictionText(QString, QString)), m_spellPredictWorker, SLOT(parsePredictionText(QString, QString)));
    connect(this, SIGNAL(addToUserWordList(QString)), m_spellPredictWorker, SLOT(addToUserWordList(QString)));
    connect(this, SIGNAL(addOverride(QString, QString)), m_spellPredictWorker, SLOT(addOverride(QString, QString)));
    m_spellPredictThread->start();
}

WesternLanguagesPlugin::~WesternLanguagesPlugin()
{
    m_spellPredictWorker->deleteLater();
    m_spellPredictThread->quit();
    m_spellPredictThread->wait();
}

void WesternLanguagesPlugin::predict(const QString& surroundingLeft, const QString& preedit)
{
    Q_EMIT parsePredictionText(surroundingLeft, preedit);
}

void WesternLanguagesPlugin::wordCandidateSelected(QString word)
{
    Q_UNUSED(word);
}

AbstractLanguageFeatures* WesternLanguagesPlugin::languageFeature()
{
    return m_languageFeatures;
}

void WesternLanguagesPlugin::spellCheckerSuggest(const QString& word, int limit)
{
    Q_EMIT setSpellCheckLimit(limit);
    Q_EMIT newSpellCheckWord(word);
}

void WesternLanguagesPlugin::addToSpellCheckerUserWordList(const QString& word)
{
    Q_EMIT addToUserWordList(word);
}

bool WesternLanguagesPlugin::setLanguage(const QString& languageId, const QString& pluginPath)
{
    Q_EMIT setSpellPredictLanguage(languageId, pluginPath);
    loadOverrides(pluginPath);
    return true;
}

void WesternLanguagesPlugin::addSpellingOverride(const QString& orig, const QString& overriden)
{
    Q_EMIT addOverride(orig, overriden);
}

void WesternLanguagesPlugin::loadOverrides(const QString& pluginPath) {
    QFile overrideFile(pluginPath + QDir::separator() + "overrides.csv");
    if (overrideFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream overrideStream(&overrideFile);
        while (!overrideStream.atEnd()) {
            QString line = overrideStream.readLine();
            QStringList components = line.split(",");
            if (components.length() == 2) {
                addSpellingOverride(components.first(), components.last());
            }
        }
    }
}
