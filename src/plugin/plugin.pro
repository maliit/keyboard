include(../config.pri)
include(../config-plugin.pri)
include(../view/view.pri)

TOP_BUILDDIR = $${OUT_PWD}/../../
TARGET = $${UBUNTU_KEYBOARD_PLUGIN_TARGET}
TEMPLATE = lib
LIBS += \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB} \
    -lubuntu_application_api \
    -lgsettings-qt
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
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
    inputmethod_p.h \
    editor.h \
    keyboadsettings.h \
    scenerectwatcher.h \
    updatenotifier.h \
    ubuntuapplicationapiwrapper.h

SOURCES += \
    plugin.cpp \
    inputmethod.cpp \
    editor.cpp \
    keyboadsettings.cpp \
    scenerectwatcher.cpp \
    updatenotifier.cpp \
    ubuntuapplicationapiwrapper.cpp

target.path += $${MALIIT_PLUGINS_DIR}
INSTALLS += target

include(../word-prediction.pri)
