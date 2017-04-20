#ifndef ROMANIANPLUGIN_H
#define ROMANIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class RomanianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.RomanianPlugin" FILE "romanianplugin.json")

public:
    explicit RomanianPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~RomanianPlugin() = default;
};

#endif // ROMANIANPLUGIN_H

