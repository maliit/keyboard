#ifndef ENGLISHPLUGIN_H
#define ENGLISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class EnglishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "englishplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit EnglishPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~EnglishPlugin() override = default;
};

#endif // ENGLISHPLUGIN_H
