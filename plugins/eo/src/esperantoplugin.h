#ifndef ESPERANTOPLUGIN_H
#define ESPERANTOPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class EsperantoPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EsperantoPlugin" FILE "esperantoplugin.json")

public:
    explicit EsperantoPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~EsperantoPlugin() override = default;
};

#endif // ESPERANTOPLUGIN_H
