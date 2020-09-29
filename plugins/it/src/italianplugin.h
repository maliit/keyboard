#ifndef ITALIANPLUGIN_H
#define ITALIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class ItalianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "italianplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit ItalianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~ItalianPlugin() override = default;
};

#endif // ITALIANPLUGIN_H
