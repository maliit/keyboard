#include "emojiplugin.h"
#include "emojilanguagefeatures.h"

#include <QDebug>

EmojiPlugin::EmojiPlugin(QObject *parent) :
    AbstractLanguagePlugin(parent)
  , m_emojiLanguageFeatures(new EmojiLanguageFeatures)
{
}

EmojiPlugin::~EmojiPlugin()
{
}

AbstractLanguageFeatures* EmojiPlugin::languageFeature()
{
    return m_emojiLanguageFeatures;
}
