#ifndef GAELICPLUGIN_H
#define GAELICPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

//#include <presage.h>

class GaelicPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.GaelicPlugin" FILE "gaelicplugin.json")

public:
    explicit GaelicPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~GaelicPlugin() override = default;
};

#endif // GAELICPLUGIN_H
