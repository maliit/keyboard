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
    Q_PLUGIN_METADATA(IID "io.maliit.keyboard.LanguagePlugin.1" FILE "japaneseplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit JapanesePlugin(QObject* parent = nullptr);
    ~JapanesePlugin() override;
    AbstractLanguageFeatures* languageFeature() override;

    void predict(const QString& surroundingLeft, const QString& preedit) override;
    void wordCandidateSelected(QString word) override;

signals:
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
