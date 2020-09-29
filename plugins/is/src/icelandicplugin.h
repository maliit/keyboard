#ifndef ICELANDICPLUGIN_H
#define ICELANDICPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class IcelandicPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "icelandicplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit IcelandicPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~IcelandicPlugin() override = default;
};

#endif // ICELANDICPLUGIN_H
