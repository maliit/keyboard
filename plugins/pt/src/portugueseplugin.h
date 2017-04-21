#ifndef PORTUGUESEPLUGIN_H
#define PORTUGUESEPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class PortuguesePlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PortuguesePlugin" FILE "portugueseplugin.json")

public:
    explicit PortuguesePlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~PortuguesePlugin() override = default;
};

#endif // PORTUGUESEPPLUGIN_H
