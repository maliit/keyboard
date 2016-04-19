#include "koreanplugin.h"
#include "koreanlanguagefeatures.h"
#include "spellpredictworker.h"

#include <QDebug>

KoreanPlugin::KoreanPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_koreanLanguageFeatures(new KoreanLanguageFeatures)
  , m_spellCheckEnabled(false)
  , m_processingSpelling(false)
{
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

}

KoreanPlugin::~KoreanPlugin()
{
    m_spellPredictWorker->deleteLater();
    m_spellPredictThread->quit();
    m_spellPredictThread->wait();
}

AbstractLanguageFeatures* KoreanPlugin::languageFeature()
{
    return m_koreanLanguageFeatures;
}

void KoreanPlugin::predict(const QString& surroundingLeft, const QString& preedit)
{
    Q_EMIT parsePredictionText(surroundingLeft, preedit);
}

void KoreanPlugin::wordCandidateSelected(QString word)
{
    Q_UNUSED(word);
}


void KoreanPlugin::spellCheckerSuggest(const QString& word, int limit)
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

void KoreanPlugin::addToSpellCheckerUserWordList(const QString& word)
{
    Q_EMIT addToUserWordList(word);
}

bool KoreanPlugin::setLanguage(const QString& languageId, const QString& pluginPath)
{
    Q_EMIT setSpellPredictLanguage(languageId, pluginPath);
    loadOverrides(pluginPath);
    return true;
}

void KoreanPlugin::addSpellingOverride(const QString& orig, const QString& overriden)
{
    Q_EMIT addOverride(orig, overriden);
}

void KoreanPlugin::loadOverrides(const QString& pluginPath) {
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

void KoreanPlugin::spellCheckFinishedProcessing(QString word, QStringList suggestions) {
    Q_EMIT newSpellingSuggestions(word, suggestions);
    if (word != m_nextSpellWord) {
        Q_EMIT newSpellCheckWord(m_nextSpellWord);
    } else {
        m_processingSpelling = false;
    }
}
