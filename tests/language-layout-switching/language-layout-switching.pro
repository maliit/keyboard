include(../../src/config.pri)
include(../common-check.pri)
include(../../src/config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
TARGET = language-layout-switching
TEMPLATE = app
QT = core testlib gui quick

!contains(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

INCLUDEPATH += ../../src/lib ../../src
LIBS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

DEFINES += TEST_MALIIT_KEYBOARD_DATADIR=\\\"$$PWD/../../data\\\"
DEFINES += TEST_QML_DATADIR=\\\"$$PWD/../../qml\\\"

HEADERS += \

SOURCES += \
    main.cpp \

include(../../src/word-prediction.pri)
