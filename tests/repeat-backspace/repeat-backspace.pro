include(../../src/config.pri)
include(../common-check.pri)
include(../../src/config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
TARGET = repeat-backspace
TEMPLATE = app
QT = core testlib gui

INCLUDEPATH += ../../src/lib ../../src
LIBS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

HEADERS += \

SOURCES += \
    main.cpp \

include(../../src/word-prediction.pri)
