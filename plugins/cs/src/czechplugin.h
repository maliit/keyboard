#ifndef CZECHPLUGIN_H
#define CZECHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class CzechPlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.CzechPlugin" FILE "czechplugin.json")

public:
    explicit CzechPlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("cs");
    }

    virtual ~CzechPlugin()
    {
    }
};

#endif // CZECHPLUGIN_H
