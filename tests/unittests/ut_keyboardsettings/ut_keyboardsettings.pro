TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_keyboardsettings
QT = core testlib

HEADERS += \
    $${TOP_SRCDIR}/src/plugin/keyboardsettings.h \
    qgsettings.h

SOURCES += \
    ut_keyboardsettings.cpp \
    $${TOP_SRCDIR}/src/plugin/keyboardsettings.cpp \
    fake_qgsettings.cpp

target.path = $$INSTALL_BIN
INSTALLS += target
