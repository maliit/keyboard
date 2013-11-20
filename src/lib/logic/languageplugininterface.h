#ifndef LANGUAGEPLUGININTERFACE_H
#define LANGUAGEPLUGININTERFACE_H

#include <QString>
#include <QStringList>

class LanguagePluginInterface
{
public:
    virtual ~LanguagePluginInterface() {}

    virtual QString parse(const QString& str) = 0;
    virtual QStringList getWordCandidates() = 0;
    virtual void wordCandidateSelected(QString word) = 0;
};

#define LanguagePluginInterface_iid "com.canonical.UbuntuKeyboard.LanguagePluginInterface"

Q_DECLARE_INTERFACE(LanguagePluginInterface, LanguagePluginInterface_iid)

#endif // LANGUAGEPLUGININTERFACE_H

