#ifndef SPANISHPLUGIN_H
#define SPANISHPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class SpanishPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "spanishplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit SpanishPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~SpanishPlugin() override = default;
};

#endif // SPANISHPLUGIN_H
