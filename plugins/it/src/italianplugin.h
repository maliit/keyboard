#ifndef ITALIANPLUGIN_H
#define ITALIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class ItalianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.ItalianPlugin" FILE "italianplugin.json")

public:
    explicit ItalianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~ItalianPlugin() override = default;
};

#endif // ITALIANPLUGIN_H
