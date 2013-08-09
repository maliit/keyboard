include(../../src/config.pri)
include(../common-check.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
TARGET = language-layout-loading
TEMPLATE = app
QT += core testlib quick

INCLUDEPATH += ../ ../../src/lib ../../src
LIBS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}
PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${UBUNTU_KEYBOARD_LIB}

DEFINES += TEST_DATADIR=\\\"$$PWD\\\"
DEFINES += TEST_MALIIT_KEYBOARD_DATADIR=\\\"$$PWD\\\"

HEADERS += \

SOURCES += \
    main.cpp \

include(../../src/word-prediction.pri)

QMAKE_EXTRA_TARGETS += xml_check
xml_check.commands = \
    xmllint --path "$$PWD/../../data/languages" --noout --dtdvalid "$$PWD/../../data/languages/VirtualKeyboardLayout.dtd" "$$PWD/languages/*.xml"

check.depends += xml_check

target.path = $$INSTALL_BIN
INSTALLS += target
