include(../config.pri)

TOP_BUILDDIR = $${OUT_PWD}/..
TEMPLATE = app
TARGET = lomiri-keyboard-benchmark
target.path = $$INSTALL_BIN

INCLUDEPATH += ../src/lib
LIBS += $${TOP_BUILDDIR}/$${LOMIRI_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${LOMIRI_KEYBOARD_LIB}
SOURCES += main.cpp

QT += core quick
INSTALLS += target

include(../word-prediction.pri)
