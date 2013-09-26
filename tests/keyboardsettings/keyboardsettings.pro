include(../../src/config.pri)
include(../common-check.pri)

TARGET = tst_keyboadsettings
TEMPLATE = app
QT = core testlib

QGSETTINGS_INCDIR = /usr/include/qt5/QGSettings

HEADERS += \
    ../../src/plugin/keyboadsettings.h \
    $${QGSETTINGS_INCDIR}/qgsettings.h \

SOURCES += \
    tst_keyboardsettings.cpp \
    ../../src/plugin/keyboadsettings.cpp \
    fake_qgsettings.cpp
