#ifndef NORWEGIANPLUGIN_H
#define NORWEGIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class NorwegianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "norwegianplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit NorwegianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~NorwegianPlugin() override = default;
};

#endif // NORWEGIANPLUGIN_H
