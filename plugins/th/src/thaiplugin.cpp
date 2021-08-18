#include "thaiplugin.h"
#include "thailanguagefeatures.h"


ThaiPlugin::ThaiPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_thaiLanguageFeatures(new ThaiLanguageFeatures(/* parent */ this))
{
}

ThaiPlugin::~ThaiPlugin()
{
}

AbstractLanguageFeatures* ThaiPlugin::languageFeature()
{
    return m_thaiLanguageFeatures;
}
