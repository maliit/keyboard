#ifndef ICELANDICPLUGIN_H
#define ICELANDICPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class IcelandicPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.IcelandicPlugin" FILE "icelandicplugin.json")

public:
    explicit IcelandicPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~IcelandicPlugin() override = default;
};

#endif // ICELANDICPLUGIN_H
