#ifndef TESTLAYOUTPLUGIN_H
#define TESTLAYOUTPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class TestLayoutPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "testlayoutplugin.json")

public:
    explicit TestLayoutPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~TestLayoutPlugin()
    {
    }
};

#endif // TESTLAYOUTPLUGIN_H
