include(../../src/config.pri)
include(../common-check.pri)
include(../../src/config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
TARGET = language-layout-switching
TEMPLATE = app
QT = core testlib gui

!contains(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

INCLUDEPATH += ../../src/lib ../../src
LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}

HEADERS += \

SOURCES += \
    main.cpp \

include(../../src/word-prediction.pri)

target.path = $$INSTALL_BIN
INSTALLS += target
