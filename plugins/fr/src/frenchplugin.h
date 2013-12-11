#ifndef FRENCHPLUGIN_H
#define FRENCHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

//#include <presage.h>

class FrenchPlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.FrenchPlugin" FILE "frenchplugin.json")

public:
    explicit FrenchPlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("fr");
    }

    virtual ~FrenchPlugin()
    {
    }
};

#endif // FRENCHPLUGIN_H
