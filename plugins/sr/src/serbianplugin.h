#ifndef SERBIANPLUGIN_H
#define SERBIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class SerbianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.SerbianPlugin" FILE "serbianplugin.json")

public:
    explicit SerbianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~SerbianPlugin() override = default;
};

#endif // SERBIANPLUGIN_H
