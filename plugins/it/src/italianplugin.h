#ifndef ITALIANPLUGIN_H
#define ITALIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class ItalianPlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.ItalianPlugin" FILE "italianplugin.json")

public:
    explicit ItalianPlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("it");
    }

    virtual ~ItalianPlugin()
    {
    }
};

#endif // ITALIANPLUGIN_H
