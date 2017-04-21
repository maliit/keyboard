#ifndef SPANISHPLUGIN_H
#define SPANISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class SpanishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.SpanishPlugin" FILE "spanishplugin.json")

public:
    explicit SpanishPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~SpanishPlugin() override = default;
};

#endif // SPANISHPLUGIN_H
