#ifndef PINYINPLUGIN_H
#define PINYINPLUGIN_H

#include <QObject>
#include <QStringList>
#include "../src/lib/logic/languageplugininterface.h"

#include "pinyinadapter.h"
#include <iostream>

class ChineseLanguageFeatures;

class PinyinPlugin : public QObject, LanguagePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PinyinPlugin" FILE "pinyinplugin.json")
    Q_INTERFACES(LanguagePluginInterface)

public:
    explicit PinyinPlugin(QObject *parent = 0);
    virtual ~PinyinPlugin();
    
    virtual void parse(const QString& surroundingLeft, const QString& preedit);
    virtual QStringList getWordCandidates();
    virtual void wordCandidateSelected(QString word);

    virtual AbstractLanguageFeatures* languageFeature();

    //! spell checker
    virtual bool spellCheckerEnabled() { return false; }
    virtual bool setSpellCheckerEnabled(bool enabled) { Q_UNUSED(enabled); return false; }
    virtual bool spell(const QString& word) { Q_UNUSED(word); return false; }
    virtual QStringList spellCheckerSuggest(const QString& word, int limit) { Q_UNUSED(word); Q_UNUSED(limit); return QStringList(); }
    virtual void addToSpellCheckerUserWordList(const QString& word) { Q_UNUSED(word); }
    virtual bool setSpellCheckerLanguage(const QString& languageId) { Q_UNUSED(languageId); return false; }

signals:
    
public slots:
    
private:
    PinyinAdapter* pinyinAdapter;
    ChineseLanguageFeatures* m_chineseLanguageFeatures;
};

#endif // PINYINPLUGIN_H
