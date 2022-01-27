#ifndef LITHUANIANPLUGIN_H
#define LITHUANIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class LithuanianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.LithuanianPlugin" FILE "lithuanianplugin.json")

public:
    explicit LithuanianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~LithuanianPlugin()
    {
    }
};

#endif // LITHUANIANPLUGIN_H
