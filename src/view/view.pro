include(../config.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
VERSION = 0.2.0
TARGET = $${UBUNTU_KEYBOARD_VIEW_TARGET}
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH = ../lib
LIBS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

contains(QT_MAJOR_VERSION, 4) {
    QT = core gui
} else {
    QT = core gui widgets
}

HEADERS += \
    setup.h \
    abstracttexteditor.h \

SOURCES += \
    setup.cpp \
    abstracttexteditor.cpp \

include(../word-prediction.pri)
