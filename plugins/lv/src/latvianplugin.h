#ifndef LATVIANPLUGIN_H
#define LATVIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class LatvianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "latvianplugin.json")

public:
    explicit LatvianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~LatvianPlugin() override = default;
};

#endif // LATVIANPLUGIN_H
