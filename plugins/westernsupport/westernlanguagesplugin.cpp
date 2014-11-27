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
    SpellPredictWorker *spellPredictWorker = new SpellPredictWorker();
    spellPredictWorker->moveToThread(m_spellPredictThread);

    connect(spellPredictWorker, SIGNAL(newSpellingSuggestions(QString, QStringList)), this, SIGNAL(newSpellingSuggestions(QString, QStringList)));
    connect(spellPredictWorker, SIGNAL(newPredictionSuggestions(QString, QStringList)), this, SIGNAL(newPredictionSuggestions(QString, QStringList)));
    connect(this, SIGNAL(newSpellCheckWord(QString)), spellPredictWorker, SLOT(newSpellCheckWord(QString)));
    connect(this, SIGNAL(setSpellPredictLanguage(QString)), spellPredictWorker, SLOT(setLanguage(QString)));
    connect(this, SIGNAL(setSpellCheckLimit(int)), spellPredictWorker, SLOT(setSpellCheckLimit(int)));
    connect(this, SIGNAL(parsePredictionText(QString, QString)), spellPredictWorker, SLOT(parsePredictionText(QString, QString)));
    connect(this, SIGNAL(addToUserWordList(QString)), spellPredictWorker, SLOT(addToUserWordList(QString)));
    connect(this, SIGNAL(addOverride(QString, QString)), spellPredictWorker, SLOT(addOverride(QString, QString)));
    m_spellPredictThread->start();
}

WesternLanguagesPlugin::~WesternLanguagesPlugin()
{
    m_spellPredictThread->quit();
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

bool WesternLanguagesPlugin::setLanguage(const QString& languageId)
{
    Q_EMIT setSpellPredictLanguage(languageId);
    return true;
}

void WesternLanguagesPlugin::addSpellingOverride(const QString& orig, const QString& overriden)
{
    Q_EMIT addOverride(orig, overriden);
}
