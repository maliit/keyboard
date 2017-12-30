include(../config.pri)
include(../config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../../
TARGET = $${UBUNTU_KEYBOARD_PLUGIN_TARGET}
TEMPLATE = lib
LIBS += \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} \
    $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB} \
    -lgsettings-qt
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
INCLUDEPATH += ../lib ../
DEFINES += MALIIT_DEFAULT_PROFILE=\\\"$$MALIIT_DEFAULT_PROFILE\\\"

contains(QT_MAJOR_VERSION, 4) {
    QT = core gui dbus
} else {
    QT = core gui widgets quick qml dbus
}

QT += eventdispatcher_support_private

CONFIG += link_pkgconfig
PKGCONFIG += mirclient

CONFIG += \
    plugin \

HEADERS += \
    plugin.h \
    inputmethod.h \
    inputmethod_p.h \
    editor.h \
    greeterstatus.h \
    keyboardgeometry.h \
    keyboardsettings.h \
    mirinputregionupdater.h \
#    updatenotifier.h \

SOURCES += \
    plugin.cpp \
    inputmethod.cpp \
    editor.cpp \
    greeterstatus.cpp \
    keyboardgeometry.cpp \
    keyboardsettings.cpp \
    mirinputregionupdater.cpp \
#    updatenotifier.cpp \

target.path += $${MALIIT_PLUGINS_DIR}
INSTALLS += target

include(../word-prediction.pri)
