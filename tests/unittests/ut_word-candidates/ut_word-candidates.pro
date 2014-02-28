TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_word-candidates
QT = core testlib gui

LIBS += \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB} \
    -lgsettings-qt
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

HEADERS += \
    wordengineprobe.h \

SOURCES += \
    wordengineprobe.cpp \
    ut_word-candidates.cpp \

include(../../../src/word-prediction.pri)

target.path = $$INSTALL_BIN
INSTALLS += target
