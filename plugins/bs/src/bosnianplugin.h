#ifndef BOSNIANPLUGIN_H
#define BOSNIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class BosnianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "bosnianplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit BosnianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~BosnianPlugin() override = default;
};

#endif // BOSNIANPLUGIN_H
