#ifndef EXAMPLEPLUGIN_H
#define EXAMPLEPLUGIN_H

#include <QObject>
#include "../src/lib/logic/languageplugininterface.h"

class ExamplePlugin : public QObject, LanguagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EchoInterface" FILE "exampleplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit ExamplePlugin(QObject *parent = 0);
    virtual ~ExamplePlugin();

    virtual void hello();
    
signals:
    
public slots:
    
};

#endif // EXAMPLEPLUGIN_H
