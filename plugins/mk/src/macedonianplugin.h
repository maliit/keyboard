#ifndef MACEDONIANPLUGIN_H
#define MACEDONIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class MacedonianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.MacedonianPlugin" FILE "macedonianplugin.json")

public:
    explicit MacedonianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~MacedonianPlugin()
    {
    }
};

#endif // MACEDONIANPLUGIN_H
