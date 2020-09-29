#ifndef CROATIANPLUGIN_H
#define CROATIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class CroatianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "croatianplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit CroatianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~CroatianPlugin() override = default;
};

#endif // CROATIANPLUGIN_H
