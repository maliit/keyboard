#ifndef GERMANTERMINALPLUGIN_H
#define GERMANTERMINALPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class GermanTerminalPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "germanterminalplugin.json")

public:
    explicit GermanTerminalPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~GermanTerminalPlugin()
    {
    }
};

#endif // GERMANTERMINALPLUGIN_H
