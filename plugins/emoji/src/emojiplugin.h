#ifndef EMOJIPLUGIN_H
#define EMOJIPLUGIN_H

#include <QObject>
#include <QStringList>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"

#include <iostream>

class EmojiLanguageFeatures;

class EmojiPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EmojiPlugin" FILE "emojiplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit EmojiPlugin(QObject *parent = nullptr);
    virtual ~EmojiPlugin();
    
    virtual AbstractLanguageFeatures* languageFeature();

private:
    EmojiLanguageFeatures* m_emojiLanguageFeatures;
};

#endif // EMOJIPLUGIN_H
