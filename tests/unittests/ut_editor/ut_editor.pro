TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_editor

QT = core gui testlib

QMAKE_LFLAGS_RPATH=$${TOP_BUILDDIR}/src/plugin

LIBS += -L$${TOP_BUILDDIR}/src/plugin -lubuntu-keyboard-plugin -lgsettings-qt

HEADERS += \
    wordengineprobe.h \

SOURCES += \
    wordengineprobe.cpp \
    ut_editor.cpp \

target.path = $$INSTALL_BIN
INSTALLS += target
