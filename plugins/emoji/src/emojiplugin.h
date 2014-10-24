#ifndef EMOJIPLUGIN_H
#define EMOJIPLUGIN_H

#include <QObject>
#include "languageplugininterface.h"
#include "westernlanguagesplugin.h"

class EmojiPlugin : public WesternLanguagesPlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EmojiPlugin" FILE "emojiplugin.json")

public:
    explicit EmojiPlugin(QObject* parent = 0)
        : WesternLanguagesPlugin(parent)
    {
    }

    virtual ~EmojiPlugin()
    {
    }
};

#endif // EMOJIPLUGIN_H
