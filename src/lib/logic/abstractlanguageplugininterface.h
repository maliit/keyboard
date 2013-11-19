#ifndef ABSTRACTLANGUAGEPLUGININTERFACE_H
#define ABSTRACTLANGUAGEPLUGININTERFACE_H

class AbstractLanguagePluginInterface
{
public:
    virtual ~AbstractLanguagePluginInterface() {}

    virtual void hello() = 0;
};

#define AbstractLanguagePluginInterface_iid "com.canonical.UbuntuKeyboard.AbstractLanguagePluginInterface"

Q_DECLARE_INTERFACE(AbstractLanguagePluginInterface, AbstractLanguagePluginInterface_iid)

#endif // ABSTRACTLANGUAGEPLUGININTERFACE_H

