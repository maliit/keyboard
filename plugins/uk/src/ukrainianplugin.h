#ifndef UKRAINIANPLUGIN_H
#define UKRAINIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class UkrainianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "ukrainianplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit UkrainianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~UkrainianPlugin() override = default;
};

#endif // UKRAINIANPLUGIN_H
