#include "westernlanguagesplugin.h"
#include "westernlanguagefeatures.h"
#include "spellchecker.h"
#include "spellcheckerworker.h"
#include "predictivetextworker.h"

#include <QDebug>

WesternLanguagesPlugin::WesternLanguagesPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_languageFeatures(new WesternLanguageFeatures)
  , m_spellChecker()
{
    m_spellCheckThread = new QThread();
    SpellCheckerWorker *spellWorker = new SpellCheckerWorker();
    spellWorker->moveToThread(m_spellCheckThread);

    connect(spellWorker, SIGNAL(newSuggestions(QStringList)), this, SIGNAL(newSpellingSuggestions(QStringList)));
    connect(this, SIGNAL(newSpellCheckWord(QString)), spellWorker, SLOT(newSpellCheckWord(QString)));
    connect(this, SIGNAL(setSpellCheckLanguage(QString)), spellWorker, SLOT(setLanguage(QString)));
    connect(this, SIGNAL(setSpellCheckLimit(int)), spellWorker, SLOT(setLimit(int)));
    connect(this, SIGNAL(spellCheckEnabled(bool)), spellWorker, SLOT(setEnabled(bool)));
    m_spellCheckThread->start();

    m_predictiveTextThread = new QThread();
    PredictiveTextWorker *predictiveWorker = new PredictiveTextWorker();
    predictiveWorker->moveToThread(m_predictiveTextThread);

    connect(predictiveWorker, SIGNAL(newSuggestions(QStringList)), this, SIGNAL(newPredictionSuggestions(QStringList)));
    connect(this, SIGNAL(parsePredictionText(QString, QString)), predictiveWorker, SLOT(parsePredictionText(QString, QString)));
    connect(this, SIGNAL(setPredictionLanguage(QString)), predictiveWorker, SLOT(setPredictionLanguage));
    m_predictiveTextThread->start();
}

WesternLanguagesPlugin::~WesternLanguagesPlugin()
{
    m_spellCheckThread->quit();
    m_predictiveTextThread->quit();
}

void WesternLanguagesPlugin::parse(const QString& surroundingLeft, const QString& preedit)
{
    Q_EMIT parsePredictionText(surroundingLeft, preedit);
}

QStringList WesternLanguagesPlugin::getWordCandidates()
{
    return QStringList();
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
    Q_EMIT spellCheckEnabled(enabled);
    return m_spellChecker.setEnabled(enabled);
}

bool WesternLanguagesPlugin::spell(const QString& word)
{
    return m_spellChecker.spell(word);
}

void WesternLanguagesPlugin::spellCheckerSuggest(const QString& word, int limit)
{
    Q_EMIT setSpellCheckLimit(limit);
    Q_EMIT newSpellCheckWord(word);
}

void WesternLanguagesPlugin::addToSpellCheckerUserWordList(const QString& word)
{
    return m_spellChecker.addToUserWordlist(word);
}

bool WesternLanguagesPlugin::setSpellCheckerLanguage(const QString& languageId)
{
    Q_EMIT setSpellCheckLanguage(languageId);
    return m_spellChecker.setLanguage(languageId);
}

void WesternLanguagesPlugin::_useDatabase(const QString &locale)
{
    Q_EMIT setPredictionLanguage(locale);
}
