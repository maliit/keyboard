#ifndef CZECHPLUGIN_H
#define CZECHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class CzechPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "czechplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit CzechPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~CzechPlugin() override = default;
};

#endif // CZECHPLUGIN_H
