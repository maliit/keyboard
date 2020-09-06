#ifndef HEBREWPLUGIN_H
#define HEBREWPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class HebrewPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "hebrewplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit HebrewPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~HebrewPlugin() override = default;
};

#endif // HEBREWPLUGIN_H
