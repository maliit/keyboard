#ifndef ENGLISHPLUGIN_H
#define ENGLISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

//#include <presage.h>

class EnglishPlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EnglishPlugin" FILE "englishplugin.json")

public:
    explicit EnglishPlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("en");
    }

    virtual ~EnglishPlugin()
    {
    }
};

#endif // ENGLISHPLUGIN_H
