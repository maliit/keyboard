#ifndef POLISHPLUGIN_H
#define POLISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class PolishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PolishPlugin" FILE "polishplugin.json")

public:
    explicit PolishPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
        _useDatabase("pl");
    }

    virtual ~PolishPlugin()
    {
    }
};

#endif // POLISHPLUGIN_H
