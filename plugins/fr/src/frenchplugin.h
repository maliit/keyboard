#ifndef FRENCHPLUGIN_H
#define FRENCHPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

//#include <presage.h>

class FrenchPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.FrenchPlugin" FILE "frenchplugin.json")

public:
    explicit FrenchPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~FrenchPlugin() override = default;
};

#endif // FRENCHPLUGIN_H
