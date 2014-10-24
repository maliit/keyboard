#ifndef BOSNIANPLUGIN_H
#define BOSNIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class BosnianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.BosnianPlugin" FILE "bosnianplugin.json")

public:
    explicit BosnianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~BosnianPlugin()
    {
    }
};

#endif // BOSNIANPLUGIN_H
