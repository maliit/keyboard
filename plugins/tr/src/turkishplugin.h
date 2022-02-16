#ifndef TURKISHPLUGIN_H
#define TURKISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class TurkishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.TurkishPlugin" FILE "turkishplugin.json")

public:
    explicit TurkishPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~TurkishPlugin()
    {
    }
};

#endif // TURKISHPLUGIN_H
