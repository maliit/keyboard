TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = ../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)


CONFIG += testlib
TARGET = ut_dynamiclayout
INCLUDEPATH += . ../../ ../../lib ../../../
QT = core testlib gui quick

LIBS += \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB} \
    -lgsettings-qt
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

DEFINES += TEST_MALIIT_KEYBOARD_DATADIR=\\\"$${TOP_SRCDIR}/data\\\"

OTHER_FILES += test-ui-constants.qml

SOURCES += ut_dynamic-layout.cpp
