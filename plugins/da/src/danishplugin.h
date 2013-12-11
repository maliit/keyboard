#ifndef DANISHPLUGIN_H
#define DANISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class DanishPlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.DanishPlugin" FILE "danishplugin.json")

public:
    explicit DanishPlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("da");
    }

    virtual ~DanishPlugin()
    {
    }
};

#endif // DANISHPLUGIN_H
