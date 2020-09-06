#ifndef FINNISHPLUGIN_H
#define FINNISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class FinnishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "finnishplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit FinnishPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~FinnishPlugin() override = default;
};

#endif // FINNISHPLUGIN_H
