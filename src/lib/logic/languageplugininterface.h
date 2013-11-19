#ifndef LANGUAGEPLUGININTERFACE_H
#define LANGUAGEPLUGININTERFACE_H

class LanguagePluginInterface
{
public:
    virtual ~LanguagePluginInterface() {}

    virtual void hello() = 0;
};

#define LanguagePluginInterface_iid "com.canonical.UbuntuKeyboard.LanguagePluginInterface"

Q_DECLARE_INTERFACE(LanguagePluginInterface, LanguagePluginInterface_iid)

#endif // LANGUAGEPLUGININTERFACE_H

