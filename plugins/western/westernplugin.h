#ifndef WESTERNPLUGIN_H
#define WESTERNPLUGIN_H

#include <QObject>
#include "../src/lib/logic/languageplugininterface.h"

#include "candidatescallback.h"

#include <presage.h>

class CandidatesCallback;

class WesternLanguagePlugin : public QObject, LanguagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.WesternLanguagePlugin" FILE "westernplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit WesternLanguagePlugin(QObject *parent = 0);
    virtual ~WesternLanguagePlugin();

    QString parse(const QString& str);
    QStringList getWordCandidates();
    void wordCandidateSelected(QString word);

signals:
    
public slots:
    
private:
    std::string candidates_context;
    CandidatesCallback presage_candidates;
    Presage presage;
};

#endif // WESTERNPLUGIN_H
