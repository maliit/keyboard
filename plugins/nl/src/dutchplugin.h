#ifndef DUTCHPLUGIN_H
#define DUTCHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class DutchPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "dutchplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit DutchPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~DutchPlugin() override = default;
};

#endif // DUTCHPLUGIN_H
