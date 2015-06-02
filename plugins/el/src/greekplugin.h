#ifndef GREEKPLUGIN_H
#define GREEKPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class GreekPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.GreekPlugin" FILE "greekplugin.json")

public:
    explicit GreekPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~GreekPlugin()
    {
    }
};

#endif // GREEKPLUGIN_H
