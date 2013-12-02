#ifndef PORTUGUESEPLUGIN_H
#define PORTUGUESEPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class PortuguesePlugin : public QObject, public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PortuguesePlugin" FILE "portugueseplugin.json")

public:
    explicit PortuguesePlugin(QObject* parent = 0)
        : QObject(parent)
        , WesternLanguagesPlugin()
    {
        _useDatabase("/home/phablet/ubuntu-keyboard/database_pt.db");
    }

    virtual ~PortuguesePlugin()
    {
    }
};

#endif // PORTUGUESEPPLUGIN_H
