#ifndef FRENCHPLUGIN_H
#define FRENCHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class FrenchPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.FrenchPlugin" FILE "frenchplugin.json")

public:
    explicit FrenchPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~FrenchPlugin()
    {
    }
};

#endif // FRENCHPLUGIN_H
