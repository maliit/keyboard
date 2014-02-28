#ifndef DUTCHPLUGIN_H
#define DUTCHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class DutchPlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.DutchPlugin" FILE "dutchplugin.json")

public:
    explicit DutchPlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("nl");
    }

    virtual ~DutchPlugin()
    {
    }
};

#endif // DUTCHPLUGIN_H
