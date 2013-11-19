#ifndef EXAMPLEPLUGIN_H
#define EXAMPLEPLUGIN_H

#include <QObject>
#include "../src/lib/logic/abstractlanguageplugininterface.h"

class ExamplePlugin : public QObject, AbstractLanguagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.EchoInterface" FILE "exampleplugin.json")
    Q_INTERFACES(AbstractLanguagePluginInterface)

public:
    explicit ExamplePlugin(QObject *parent = 0);
    virtual ~ExamplePlugin();

    virtual void hello();
    
signals:
    
public slots:
    
};

#endif // EXAMPLEPLUGIN_H
