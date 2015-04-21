#ifndef TESTLAYOUTPLUGIN_H
#define TESTLAYOUTPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class TestLayoutPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.TestLayoutPlugin" FILE "testlayoutplugin.json")

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
