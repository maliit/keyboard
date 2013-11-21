#include "westernplugin.h"
#include "languagefeatures.h"

#include <QDebug>

WesternLanguagePlugin::WesternLanguagePlugin(QObject *parent) :
    QObject(parent)
  , candidates_context()
  , presage_candidates(CandidatesCallback(candidates_context))
  , presage(&presage_candidates)
  , m_languageFeatures(new LanguageFeatures)
{
    presage.config("Presage.Selector.SUGGESTIONS", "6");
    presage.config("Presage.Selector.REPEAT_SUGGESTIONS", "yes");
}

WesternLanguagePlugin::~WesternLanguagePlugin()
{
}

// TODO no return
QString WesternLanguagePlugin::parse(const QString& str)
{
    candidates_context = str.toStdString();

    return str;
}

QStringList WesternLanguagePlugin::getWordCandidates()
{
    const std::vector<std::string> predictions = presage.predict();

    QStringList list;
    std::vector<std::string>::const_iterator it;
    for (it = predictions.begin(); it != predictions.end(); ++it)
        list << QString::fromStdString(*it);

    return list;
}

void WesternLanguagePlugin::wordCandidateSelected(QString word)
{

}

AbstractLanguageFeatures* WesternLanguagePlugin::languageFeature()
{
    return m_languageFeatures;
}
