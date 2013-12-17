TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_de.path = "$$UBUNTU_KEYBOARD_LIB_DIR/de/"
lang_de.files = *.qml *.js

INSTALLS += lang_de

# for QtCreator
OTHER_FILES += \
    Keyboard_de.qml \
    Keyboard_de_email.qml \
    Keyboard_de_url.qml \
    Keyboard_de_url_search.qml

