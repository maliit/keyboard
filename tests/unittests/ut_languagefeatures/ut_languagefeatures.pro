TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_languagefeatures
QT = core testlib

QMAKE_LFLAGS_RPATH=$${TOP_BUILDDIR}/src/plugin
LIBS += -L$${TOP_BUILDDIR}/src/plugin -lubuntu-keyboard-plugin

INCLUDEPATH    += \
    $${TOP_SRCDIR}/src/lib/ \
    $${TOP_SRCDIR}/src/lib/logic/


#HEADERS += $${TOP_SRCDIR}/src/lib/logic/westernlanguagefeatures.h

SOURCES += \
#    $${TOP_SRCDIR}/src/lib/logic/westernlanguagefeatures.cpp \
    ut_languagefeatures.cpp

target.path = $$INSTALL_BIN
INSTALLS += target

LIBS += -L$$TOP_BUILDDIR/plugins/plugins -lwesternsupport

INCLUDEPATH += $$TOP_SRCDIR/plugins/westernsupport
DEPENDPATH += $$TOP_SRCDIR/plugins/westernsupport
