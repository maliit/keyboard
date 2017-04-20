#ifndef CATALANPLUGIN_H
#define CATALANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class CatalanPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.CatalanPlugin" FILE "catalanplugin.json")

public:
    explicit CatalanPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~CatalanPlugin() = default;
};

#endif // CATALANPLUGIN_H
