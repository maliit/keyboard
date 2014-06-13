#ifndef HUNGARIANPLUGIN_H
#define HUNGARIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class HungarianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.HungarianPlugin" FILE "hungarianplugin.json")

public:
    explicit HungarianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~HungarianPlugin()
    {
    }
};

#endif // HUNGARIANPLUGIN_H
