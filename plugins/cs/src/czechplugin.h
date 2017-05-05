#ifndef CZECHPLUGIN_H
#define CZECHPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class CzechPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.CzechPlugin" FILE "czechplugin.json")

public:
    explicit CzechPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~CzechPlugin() override = default;
};

#endif // CZECHPLUGIN_H
