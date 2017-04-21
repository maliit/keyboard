#ifndef SERBIANPLUGIN_H
#define SERBIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class SerbianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.SerbianPlugin" FILE "serbianplugin.json")

public:
    explicit SerbianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~SerbianPlugin() = default;
};

#endif // SERBIANPLUGIN_H
