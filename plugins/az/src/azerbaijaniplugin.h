#ifndef AZERBAIJANIPLUGIN_H
#define AZERBAIJANIPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

//#include <presage.h>

class AzerbaijaniPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "azerbaijaniplugin.json")

public:
    explicit AzerbaijaniPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~AzerbaijaniPlugin() override = default;
};

#endif // AZERBAIJANIPLUGIN_H
