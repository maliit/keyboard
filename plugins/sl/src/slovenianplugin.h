#ifndef SLOVENIANPLUGIN_H
#define SLOVENIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class SlovenianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "slovenianplugin.json")

public:
    explicit SlovenianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~SlovenianPlugin() override = default;
};

#endif // SLOVENIANPLUGIN_H
