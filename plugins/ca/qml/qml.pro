TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_ca.path = "$${LOMIRI_KEYBOARD_LIB_DIR}/ca/"
lang_ca.files = *.qml *.js

INSTALLS += lang_ca

# for QtCreator
OTHER_FILES += \
    Keyboard_ca.qml \
    Keyboard_ca_email.qml \
    Keyboard_ca_url.qml \
    Keyboard_ca_url_search.qml

