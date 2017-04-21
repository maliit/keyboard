#ifndef LATVIANPLUGIN_H
#define LATVIANPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class LatvianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.LatvianPlugin" FILE "latvianplugin.json")

public:
    explicit LatvianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~LatvianPlugin() = default;
};

#endif // LATVIANPLUGIN_H
