#ifndef HUNGARIANPLUGIN_H
#define HUNGARIANPLUGIN_H

#include <QObject>
#include "westernlanguagesplugin.h"

class HungarianPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "hungarianplugin.json")

public:
    explicit HungarianPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~HungarianPlugin() override = default;
};

#endif // HUNGARIANPLUGIN_H
