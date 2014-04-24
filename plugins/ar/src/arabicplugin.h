#ifndef ARABICPLUGIN_H
#define ARABICPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class ArabicPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.ArabicPlugin" FILE "arabicplugin.json")

public:
    explicit ArabicPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
        _useDatabase("ar");
    }

    virtual ~ArabicPlugin()
    {
    }
};

#endif // ARABICPLUGIN_H
