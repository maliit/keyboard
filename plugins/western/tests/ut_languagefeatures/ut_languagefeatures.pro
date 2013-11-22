TOP_BUILDDIR = $$OUT_PWD/../../../..
TOP_SRCDIR = $$PWD/../../../..
PLUGIN_SRCDIR = $$PWD/plugins/western

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_languagefeatures
QT = core testlib

QMAKE_LFLAGS_RPATH=$${TOP_BUILDDIR}/src/plugin
LIBS += -L$${TOP_BUILDDIR}/src/plugin -lubuntu-keyboard-plugin

INCLUDEPATH    += \
    $${PLUGIN_SRCDIR}/src/ \
    $${TOP_SRCDIR}/src/lib/ \
    $${TOP_SRCDIR}/src/lib/logic/


HEADERS += $${PLUGIN_SRCDIR}/src/languagefeatures.h

SOURCES += \
    ut_languagefeatures.cpp

target.path = $$INSTALL_BIN
INSTALLS += target
