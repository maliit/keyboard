include(../config.pri)
include(../view/view.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
TARGET = maliit-keyboard-viewer
TEMPLATE = app

INCLUDEPATH += ../lib ../
LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}

HEADERS += \
    dashboard.h \

SOURCES += \
    main.cpp \
    dashboard.cpp \

contains(QT_MAJOR_VERSION, 4) {
    QT = core gui
} else {
    QT = core gui widgets
}

target.path = $$INSTALL_BIN
INSTALLS += target

include(../word-prediction.pri)
