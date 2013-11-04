#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>
#include <qdebug.h>
#include <qcoreapplication.h>

#include <QStringList>
#include <QUrl>
#include <QFile>

class MaliitTestHelper : public QObject
{
    Q_OBJECT

public:
    MaliitTestHelper(QObject *parent=0) : QObject(parent)
    {
    }

    ~MaliitTestHelper()
    {
    }

    Q_INVOKABLE QStringList localeUiLanguages(QVariant value) {
        QLocale locale = value.toLocale();
        return locale.uiLanguages();
    }

    Q_INVOKABLE QString localeName(QVariant value) {
        QLocale locale = value.toLocale();
        return locale.name();
    }

signals:
private:
};


class MaliitQmlHelperPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:

    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("MaliitTestSupport"));
        qmlRegisterType<MaliitTestHelper>(uri, 1, 0, "MaliitTestHelper");
    }
};


#include "plugin.moc"


