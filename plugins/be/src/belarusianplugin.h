#ifndef BELARUSIANPLUGIN_H
#define BELARUSIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class BelarusianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.BelarusianPlugin" FILE "belarusianplugin.json")

public:
    explicit BelarusianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~BelarusianPlugin()
    {
    }
};

#endif // BELARUSIANPLUGIN_H
