#ifndef ICELANDICPLUGIN_H
#define ICELANDICPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class IcelandicPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.IcelandicPlugin" FILE "icelandicplugin.json")

public:
    explicit IcelandicPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~IcelandicPlugin() = default;
};

#endif // ICELANDICPLUGIN_H
