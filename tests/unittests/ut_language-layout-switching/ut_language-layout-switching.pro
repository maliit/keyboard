TOP_BUILDDIR = $${OUT_PWD}/../../..
TOP_SRCDIR = ../../..

include($${TOP_SRCDIR}/config.pri)
include(../common-check.pri)

CONFIG += testcase
TARGET = ut_language-layout-switching
QT = core testlib gui quick

LIBS += \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB} \
    -lgsettings-qt
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

DEFINES += TEST_MALIIT_KEYBOARD_DATADIR=\\\"$${TOP_SRCDIR}/data\\\"
DEFINES += TEST_QML_DATADIR=\\\"$${TOP_SRCDIR}/qml\\\"

HEADERS += \

SOURCES += \
    ut_language-layout-switching.cpp \

target.path = $$INSTALL_BIN
INSTALLS += target
