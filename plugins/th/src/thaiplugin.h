#ifndef THAIPLUGIN_H
#define THAIPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"

class ThaiLanguageFeatures;

class ThaiPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.lomiri.LomiriKeyboard.LanguagePluginInterface" FILE "thaiplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit ThaiPlugin(QObject *parent = 0);
    virtual ~ThaiPlugin();
    
    virtual AbstractLanguageFeatures* languageFeature();

private:
    ThaiLanguageFeatures* m_thaiLanguageFeatures;
};

#endif // THAIPLUGIN_H
