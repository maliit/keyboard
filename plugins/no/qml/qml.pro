TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_no.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/no/"
lang_no.files = *.qml *.js

INSTALLS += lang_no

# for QtCreator
OTHER_FILES += \
    Keyboard_no.qml \
    Keyboard_no_email.qml \
    Keyboard_no_url.qml \
    Keyboard_no_url_search.qml

