TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_mk.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/mk/"
lang_mk.files = *.qml *.js

INSTALLS += lang_mk

# for QtCreator
OTHER_FILES += \
    Keyboard_mk.qml \
    Keyboard_mk_email.qml \
    Keyboard_mk_url.qml \
    Keyboard_mk_url_search.qml

