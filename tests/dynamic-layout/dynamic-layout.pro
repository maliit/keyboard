include(../../config.pri)
include(../common-check.pri)
include(../../src/config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
#CONFIG += testlib
TEMPLATE = app
TARGET = dynamic-layout
INCLUDEPATH += . ../ ../../lib ../../
QT = core testlib gui quick

INCLUDEPATH += ../../src/lib ../../src/
LIBS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

DEFINES += TEST_MALIIT_KEYBOARD_DATADIR=\\\"$$PWD/../../data\\\"

OTHER_FILES += test-ui-constants.qml

SOURCES += test.cpp
