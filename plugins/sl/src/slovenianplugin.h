#ifndef SLOVENIANPLUGIN_H
#define SLOVENIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class SlovenianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.SlovenianPlugin" FILE "slovenianplugin.json")

public:
    explicit SlovenianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~SlovenianPlugin() override = default;
};

#endif // SLOVENIANPLUGIN_H
