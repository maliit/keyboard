#ifndef CROATIANPLUGIN_H
#define CROATIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class CroatianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.CroatianPlugin" FILE "croatianplugin.json")

public:
    explicit CroatianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~CroatianPlugin() = default;
};

#endif // CROATIANPLUGIN_H
