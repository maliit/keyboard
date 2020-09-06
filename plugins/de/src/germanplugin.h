#ifndef GERMANPLUGIN_H
#define GERMANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class GermanPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "germanplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit GermanPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~GermanPlugin() override = default;
};

#endif // GERMANPLUGIN_H
