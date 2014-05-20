#ifndef GERMANPLUGIN_H
#define GERMANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class GermanPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.GermanPlugin" FILE "germanplugin.json")

public:
    explicit GermanPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
        _useDatabase("de");
    }

    virtual ~GermanPlugin()
    {
    }
};

#endif // GERMANPLUGIN_H
