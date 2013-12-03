include(../../../src/config-plugin.pri)

TARGET = tests-common
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
           utils.cpp \
           utils-gui.cpp \
           inputmethodhostprobe.cpp \
           mockscreen.cpp \
           wordengineprobe.cpp \

HEADERS += \
           utils.h \
           inputmethodhostprobe.h \
           mockscreen.h \
           wordengineprobe.h \

contains(QT_MAJOR_VERSION, 4) {
    QT = core gui
} else {
    QT = core gui widgets
}
INSTALLS += target
INCLUDEPATH += ../../../src/lib ../../src/
LIBS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

QMAKE_EXTRA_TARGETS += check
check.target = check
check.command = $$system(true)
check.depends += libtests-common.a

OTHER_FILES += \
    helper.js

