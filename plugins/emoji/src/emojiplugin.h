#ifndef EMOJIPLUGIN_H
#define EMOJIPLUGIN_H

#include <QObject>
#include <QStringList>
#include "abstractlanguageplugin.h"

#include <iostream>

class EmojiLanguageFeatures;

class EmojiPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "emojiplugin.json")

public:
    explicit EmojiPlugin(QObject *parent = nullptr);
    ~EmojiPlugin() override;
    
    AbstractLanguageFeatures* languageFeature() override;

private:
    EmojiLanguageFeatures* m_emojiLanguageFeatures;
};

#endif // EMOJIPLUGIN_H
