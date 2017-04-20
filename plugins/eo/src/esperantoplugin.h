#ifndef ESPERANTOPLUGIN_H
#define ESPERANTOPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class EsperantoPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EsperantoPlugin" FILE "esperantoplugin.json")

public:
    explicit EsperantoPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~EsperantoPlugin() = default;
};

#endif // ESPERANTOPLUGIN_H
