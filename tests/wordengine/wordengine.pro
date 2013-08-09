include(../../config.pri)
include(../common-check.pri)
include(../../config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
CONFIG += testlib
TEMPLATE = app
TARGET = ut_wordengine
INCLUDEPATH += . ../ ../../lib ../../
QT = core testlib gui

INCLUDEPATH += ../../src/lib ../../src/
LIBS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

SOURCES += main.cpp
