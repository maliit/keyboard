#ifndef RUSSIANPLUGIN_H
#define RUSSIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class RussianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.RussianPlugin" FILE "russianplugin.json")

public:
    explicit RussianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~RussianPlugin() override = default;
};

#endif // RUSSIANPLUGIN_H
