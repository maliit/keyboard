#ifndef UKRAINIANPLUGIN_H
#define UKRAINIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class UkrainianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.UkrainianPlugin" FILE "ukrainianplugin.json")

public:
    explicit UkrainianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~UkrainianPlugin() override = default;
};

#endif // UKRAINIANPLUGIN_H
