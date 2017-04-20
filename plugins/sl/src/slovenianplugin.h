#ifndef SLOVENIANPLUGIN_H
#define SLOVENIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class SlovenianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.SlovenianPlugin" FILE "slovenianplugin.json")

public:
    explicit SlovenianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~SlovenianPlugin() = default;
};

#endif // SLOVENIANPLUGIN_H
