#ifndef KOREANPLUGIN_H
#define KOREANPLUGIN_H

#include <QObject>
#include <QStringList>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"

#include <iostream>

class KoreanLanguageFeatures;

class KoreanPlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.KoreanPlugin" FILE "koreanplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit KoreanPlugin(QObject *parent = 0);
    virtual ~KoreanPlugin();

    virtual AbstractLanguageFeatures* languageFeature();

private:
    KoreanLanguageFeatures* m_koreanLanguageFeatures;
};

#endif // KOREANPLUGIN_H
