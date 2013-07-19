include(../config.pri)
include(../config-plugin.pri)
include(../view/view.pri)

TOP_BUILDDIR = $${OUT_PWD}/../../
TARGET = $${MALIIT_KEYBOARD_PLUGIN_TARGET}
TEMPLATE = lib
LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB} -lubuntu_application_api
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}
INCLUDEPATH += ../lib ../
DEFINES += MALIIT_DEFAULT_PROFILE=\\\"$$MALIIT_DEFAULT_PROFILE\\\"

contains(QT_MAJOR_VERSION, 4) {
    QT = core gui
} else {
    QT = core gui widgets quick qml
}

CONFIG += \
    plugin \

HEADERS += \
    plugin.h \
    inputmethod.h \
    editor.h \
    updatenotifier.h \
    maliitcontext.h \

SOURCES += \
    plugin.cpp \
    inputmethod.cpp \
    editor.cpp \
    updatenotifier.cpp \
    maliitcontext.cpp \

target.path += $${MALIIT_PLUGINS_DIR}
INSTALLS += target

include(../word-prediction.pri)
