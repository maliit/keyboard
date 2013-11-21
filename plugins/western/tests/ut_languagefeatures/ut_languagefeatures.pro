TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_languagefeatures
QT = core testlib

QMAKE_LFLAGS_RPATH=$${TOP_BUILDDIR}/src/plugin
LIBS += -L$${TOP_BUILDDIR}/src/plugin -lubuntu-keyboard-plugin

HEADERS += \
    $${TOP_SRCDIR}/src/lib/logic/languagefeatures.h

SOURCES += \
    ut_languagefeatures.cpp

target.path = $$INSTALL_BIN
INSTALLS += target
