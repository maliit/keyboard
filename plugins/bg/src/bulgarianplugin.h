#ifndef BULGARIANPLUGIN_H
#define BULGARIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class BulgarianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.BulgarianPlugin" FILE "bulgarianplugin.json")

public:
    explicit BulgarianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~BulgarianPlugin()
    {
    }
};

#endif // BULGARIANPLUGIN_H
