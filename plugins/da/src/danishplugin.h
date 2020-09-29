#ifndef DANISHPLUGIN_H
#define DANISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class DanishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "danishplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit DanishPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~DanishPlugin() override = default;
};

#endif // DANISHPLUGIN_H
