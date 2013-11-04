include(../../../src/config-plugin.pri)

TARGET = maliitqmlhelperplugin
DESTDIR = imports/MaliitTestSupport
TEMPLATE = lib
CONFIG += qt plugin

SOURCES += \
    plugin.cpp

QT = core gui qml quick qml-private

INCLUDEPATH += ../../../../lib ../../

pluginfiles.files += \
    imports/MaliitTestSupport/qmldir \

OTHER_FILES += \
    tst_inputMethodHints.qml

