#ifndef FINNISHPLUGIN_H
#define FINNISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class FinnishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.FinnishPlugin" FILE "finnishplugin.json")

public:
    explicit FinnishPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~FinnishPlugin() = default;
};

#endif // FINNISHPLUGIN_H
