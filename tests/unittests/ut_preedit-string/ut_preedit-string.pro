TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = ../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_preedit-string
QT = core testlib gui

LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}

SOURCES += \
    ut_preedit-string.cpp \

target.path = $$INSTALL_BIN
INSTALLS += target
