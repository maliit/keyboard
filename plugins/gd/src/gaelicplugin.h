#ifndef GAELICPLUGIN_H
#define GAELICPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class GaelicPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "gaelicplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit GaelicPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~GaelicPlugin() override = default;
};

#endif // GAELICPLUGIN_H
