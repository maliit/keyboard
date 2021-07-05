TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_el.path = "$${LOMIRI_KEYBOARD_LIB_DIR}/el/"
lang_el.files = *.qml *.js

INSTALLS += lang_el

# for QtCreator
OTHER_FILES += \
    Keyboard_el.qml \
    Keyboard_el_email.qml \
    Keyboard_el_url.qml \
    Keyboard_el_url_search.qml

