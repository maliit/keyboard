TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_cs.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/cs/"
lang_cs.files = *.qml *.js

INSTALLS += lang_cs

# for QtCreator
OTHER_FILES += \
    Keyboard_cs.qml \
    Keyboard_cs_email.qml \
    Keyboard_cs_url.qml \
    Keyboard_cs_url_search.qml

