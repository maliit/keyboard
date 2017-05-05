#ifndef ROMANIANPLUGIN_H
#define ROMANIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class RomanianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.RomanianPlugin" FILE "romanianplugin.json")

public:
    explicit RomanianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~RomanianPlugin() override = default;
};

#endif // ROMANIANPLUGIN_H

