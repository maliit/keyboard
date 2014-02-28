TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_nl.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/nl/"
lang_nl.files = *.qml *.js

INSTALLS += lang_nl

# for QtCreator
OTHER_FILES += \
    Keyboard_nl.qml \
    Keyboard_nl_email.qml \
    Keyboard_nl_url.qml \
    Keyboard_nl_url_search.qml

