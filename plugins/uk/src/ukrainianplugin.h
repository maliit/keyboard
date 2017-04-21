#ifndef UKRAINIANPLUGIN_H
#define UKRAINIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class UkrainianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.UkrainianPlugin" FILE "ukrainianplugin.json")

public:
    explicit UkrainianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~UkrainianPlugin() = default;
};

#endif // UKRAINIANPLUGIN_H
