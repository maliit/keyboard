#ifndef SWEDISHPLUGIN_H
#define SWEDISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class SwedishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "swedishplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit SwedishPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~SwedishPlugin() override = default;
};

#endif // SWEDISHPLUGIN_H
