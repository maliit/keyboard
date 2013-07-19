include(../../config.pri)
include(../common-check.pri)

TOP_BUILDDIR = $${OUT_PWD}/../../..
TARGET = rendering
TEMPLATE = app
QT = core testlib gui

!contains(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

INCLUDEPATH += ../ ../../lib ../../
LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}

HEADERS += \

SOURCES += \
    main.cpp \

include(../../word-prediction.pri)

