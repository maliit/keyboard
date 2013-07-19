include(../../config.pri)
include(../common-check.pri)
include(../../config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../../..
TARGET = repeat-backspace
TEMPLATE = app
QT = core testlib gui

INCLUDEPATH += ../../lib ../../
LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}

HEADERS += \

SOURCES += \
    main.cpp \

include(../../word-prediction.pri)
