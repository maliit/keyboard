#ifndef CATALANPLUGIN_H
#define CATALANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class CatalanPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.CatalanPlugin" FILE "catalanplugin.json")

public:
    explicit CatalanPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~CatalanPlugin() override = default;
};

#endif // CATALANPLUGIN_H
