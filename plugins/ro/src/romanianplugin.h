#ifndef ROMANIANPLUGIN_H
#define ROMANIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class RomanianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "romanianplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit RomanianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~RomanianPlugin() override = default;
};

#endif // ROMANIANPLUGIN_H
