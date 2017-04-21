#ifndef JAPANESEPLUGIN_H
#define JAPANESEPLUGIN_H

#include <QObject>
#include <QThread>
#include "languageplugininterface.h"
#include "abstractlanguageplugin.h"

#include "anthyadapter.h"

class JapaneseLanguageFeatures;

class JapanesePlugin : public AbstractLanguagePlugin
{
    Q_OBJECT
    Q_INTERFACES(LanguagePluginInterface)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.JapanesePlugin" FILE "japaneseplugin.json")

public:
    explicit JapanesePlugin(QObject* parent = nullptr);
    virtual ~JapanesePlugin();
    virtual AbstractLanguageFeatures* languageFeature();

    virtual void predict(const QString& surroundingLeft, const QString& preedit);
    virtual void wordCandidateSelected(QString word);

signals:
    void newPredictionSuggestions(QString word, QStringList suggestions);
    void parsePredictionText(QString preedit);
    void candidateSelected(QString word);

public slots:
    void finishedProcessing(QString word, QStringList suggestions);

private:
    JapaneseLanguageFeatures* m_japaneseLanguageFeatures;
    QThread *m_anthyThread;
    AnthyAdapter *m_anthyAdapter;
    QString m_nextWord;
    bool m_processingWord;
};

#endif // JAPANESEPLUGIN_H
