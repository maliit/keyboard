#ifndef NORWEGIANPLUGIN_H
#define NORWEGIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class NorwegianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.NorwegianPlugin" FILE "norwegianplugin.json")

public:
    explicit NorwegianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~NorwegianPlugin()
    {
    }
};

#endif // NORWEGIANPLUGIN_H
