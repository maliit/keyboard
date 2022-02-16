#include "westernlanguagesplugin.h"
#include "westernlanguagefeatures.h"
#include "spellpredictworker.h"

#include <QDebug>

WesternLanguagesPlugin::WesternLanguagesPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_languageFeatures(new WesternLanguageFeatures)
  , m_spellPredictWorker(nullptr)
  , m_spellPredictThread(nullptr)
  , m_spellCheckEnabled(false)
  , m_processingSpelling(false)
{
    m_spellPredictThread = new QThread();
    m_spellPredictWorker = new SpellPredictWorker();
    m_spellPredictWorker->moveToThread(m_spellPredictThread);

    connect(m_spellPredictWorker, &SpellPredictWorker::newSpellingSuggestions, this, &WesternLanguagesPlugin::spellCheckFinishedProcessing);
    connect(m_spellPredictWorker, &SpellPredictWorker::newPredictionSuggestions, this, &WesternLanguagesPlugin::newPredictionSuggestions);
    connect(this, &WesternLanguagesPlugin::newSpellCheckWord, m_spellPredictWorker, &SpellPredictWorker::newSpellCheckWord);
    connect(this, &WesternLanguagesPlugin::setSpellPredictLanguage, m_spellPredictWorker, &SpellPredictWorker::setLanguage);
    connect(this, &WesternLanguagesPlugin::setSpellCheckLimit, m_spellPredictWorker, &SpellPredictWorker::setSpellCheckLimit);
    connect(this, &WesternLanguagesPlugin::parsePredictionText, m_spellPredictWorker, &SpellPredictWorker::parsePredictionText);
    connect(this, &WesternLanguagesPlugin::addToUserWordList, m_spellPredictWorker, &SpellPredictWorker::addToUserWordList);
    connect(this, &WesternLanguagesPlugin::addOverride, m_spellPredictWorker, &SpellPredictWorker::addOverride);
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
    m_nextSpellWord = word;
    // Don't accept new words whilst we're processing, so we only process the
    // most recent input once the current processing has completed
    if (!m_processingSpelling) {
        m_processingSpelling = true;
        Q_EMIT setSpellCheckLimit(limit);
        Q_EMIT newSpellCheckWord(word);
    }
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

void WesternLanguagesPlugin::addSpellingOverride(const QString& orig, const QString& overridden)
{
    Q_EMIT addOverride(orig, overridden);
}

void WesternLanguagesPlugin::loadOverrides(const QString& pluginPath) {
    QFile overrideFile(pluginPath + QDir::separator() + "overrides.csv");
    if (overrideFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream overrideStream(&overrideFile);
        while (!overrideStream.atEnd()) {
            QString line = overrideStream.readLine();
            QStringList components = line.split(QStringLiteral(","));
            if (components.length() == 2) {
                addSpellingOverride(components.first(), components.last());
            }
        }
    }
}

void WesternLanguagesPlugin::spellCheckFinishedProcessing(QString word, QStringList suggestions) {
    Q_EMIT newSpellingSuggestions(word, suggestions);
    if (word != m_nextSpellWord) {
        Q_EMIT newSpellCheckWord(m_nextSpellWord);
    } else {
        m_processingSpelling = false;
    }
}
