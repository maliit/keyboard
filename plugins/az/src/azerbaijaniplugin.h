#ifndef AZERBAIJANIPLUGIN_H
#define AZERBAIJANIPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

//#include <presage.h>

class AzerbaijaniPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.AzerbaijaniPlugin" FILE "azerbaijaniplugin.json")

public:
    explicit AzerbaijaniPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~AzerbaijaniPlugin() = default;
};

#endif // AZERBAIJANIPLUGIN_H
