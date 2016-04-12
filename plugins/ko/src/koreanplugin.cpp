#include "koreanplugin.h"
#include "koreanlanguagefeatures.h"

#include <QDebug>

KoreanPlugin::KoreanPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_koreanLanguageFeatures(new KoreanLanguageFeatures)
{
}

KoreanPlugin::~KoreanPlugin()
{
}

AbstractLanguageFeatures* KoreanPlugin::languageFeature()
{
    return m_koreanLanguageFeatures;
}
