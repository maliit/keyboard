TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_keyboardgeometry
QT = core testlib

HEADERS += \
    $${TOP_SRCDIR}/src/plugin/keyboardgeometry.h

SOURCES += \
    ut_keyboardgeometry.cpp \
    $${TOP_SRCDIR}/src/plugin/keyboardgeometry.cpp

target.path = $$INSTALL_BIN
INSTALLS += target
