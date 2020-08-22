#ifndef GAELICPLUGIN_H
#define GAELICPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

//#include <presage.h>

class GaelicPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "gaelicplugin.json")

public:
    explicit GaelicPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~GaelicPlugin() override = default;
};

#endif // GAELICPLUGIN_H
