TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_it.path = "$$UBUNTU_KEYBOARD_LIB_DIR/it/"
lang_it.files = *.qml *.js

INSTALLS += lang_it

# for QtCreator
OTHER_FILES += \
    Keyboard_it.qml \
    Keyboard_it_email.qml \
    Keyboard_it_url.qml \
    Keyboard_it_url_search.qml

