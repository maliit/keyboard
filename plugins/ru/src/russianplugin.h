#ifndef RUSSIANPLUGIN_H
#define RUSSIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class RussianPlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.RussianPlugin" FILE "russianplugin.json")

public:
    explicit RussianPlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("ru");
    }

    virtual ~RussianPlugin()
    {
    }
};

#endif // RUSSIANPLUGIN_H
