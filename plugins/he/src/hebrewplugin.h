#ifndef HEBREWPLUGIN_H
#define HEBREWPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class HebrewPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.HebrewPlugin" FILE "hebrewplugin.json")

public:
    explicit HebrewPlugin(QObject* parent = nullptr)
        : WesternLanguagesPlugin(parent)
    {
    }

    ~HebrewPlugin() override = default;
};

#endif // HEBREWPLUGIN_H
