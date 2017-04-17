#include "westernlanguagesplugin.h"
#include "westernlanguagefeatures.h"

#ifdef HAVE_PRESAGE
#include "spellpredictworker.h"
#endif

#include <QDebug>

WesternLanguagesPlugin::WesternLanguagesPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_languageFeatures(new WesternLanguageFeatures)
  , m_spellPredictWorker(nullptr)
  , m_spellPredictThread(nullptr)
  , m_spellCheckEnabled(false)
  , m_processingSpelling(false)
{
#ifdef HAVE_PRESAGE
    m_spellPredictThread = new QThread();
    m_spellPredictWorker = new SpellPredictWorker();
    m_spellPredictWorker->moveToThread(m_spellPredictThread);

    connect(m_spellPredictWorker, SIGNAL(newSpellingSuggestions(QString, QStringList)), this, SLOT(spellCheckFinishedProcessing(QString, QStringList)));
    connect(m_spellPredictWorker, SIGNAL(newPredictionSuggestions(QString, QStringList)), this, SIGNAL(newPredictionSuggestions(QString, QStringList)));
    connect(this, SIGNAL(newSpellCheckWord(QString)), m_spellPredictWorker, SLOT(newSpellCheckWord(QString)));
    connect(this, SIGNAL(setSpellPredictLanguage(QString, QString)), m_spellPredictWorker, SLOT(setLanguage(QString, QString)));
    connect(this, SIGNAL(setSpellCheckLimit(int)), m_spellPredictWorker, SLOT(setSpellCheckLimit(int)));
    connect(this, SIGNAL(parsePredictionText(QString, QString)), m_spellPredictWorker, SLOT(parsePredictionText(QString, QString)));
    connect(this, SIGNAL(addToUserWordList(QString)), m_spellPredictWorker, SLOT(addToUserWordList(QString)));
    connect(this, SIGNAL(addOverride(QString, QString)), m_spellPredictWorker, SLOT(addOverride(QString, QString)));
    m_spellPredictThread->start();
#endif
}

WesternLanguagesPlugin::~WesternLanguagesPlugin()
{
#ifdef HAVE_PRESAGE
    m_spellPredictWorker->deleteLater();
    m_spellPredictThread->quit();
    m_spellPredictThread->wait();
#endif
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

void WesternLanguagesPlugin::spellCheckFinishedProcessing(QString word, QStringList suggestions) {
    Q_EMIT newSpellingSuggestions(word, suggestions);
    if (word != m_nextSpellWord) {
        Q_EMIT newSpellCheckWord(m_nextSpellWord);
    } else {
        m_processingSpelling = false;
    }
}
