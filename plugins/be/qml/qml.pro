TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_be.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/be/"
lang_be.files = *.qml *.js

INSTALLS += lang_be

# for QtCreator
OTHER_FILES += \
    Keyboard_be.qml \
    Keyboard_be_email.qml \
    Keyboard_be_url.qml \
    Keyboard_be_url_search.qml

