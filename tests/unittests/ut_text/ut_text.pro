TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_text
QT = core testlib

QMAKE_LFLAGS_RPATH=$${TOP_BUILDDIR}/src/plugin
LIBS += -L$${TOP_BUILDDIR}/src/plugin -lubuntu-keyboard-plugin

HEADERS += \
    $${TOP_SRCDIR}/src/lib/models/text.h

SOURCES += \
    ut_text.cpp

target.path = $$INSTALL_BIN
INSTALLS += target
