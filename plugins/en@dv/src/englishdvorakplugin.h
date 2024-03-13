#ifndef ENGLISHDVORAKPLUGIN_H
#define ENGLISHDVORAKPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class EnglishDvorakPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EnglishDvorakPlugin" FILE "englishdvorakplugin.json")

public:
    explicit EnglishDvorakPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~EnglishDvorakPlugin()
    {
    }
};

#endif // ENGLISHDVORAKPLUGIN_H
