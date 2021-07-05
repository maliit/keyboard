TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_sr.path = "$${LOMIRI_KEYBOARD_LIB_DIR}/sr/"
lang_sr.files = *.qml *.js

INSTALLS += lang_sr

# for QtCreator
OTHER_FILES += \
    Keyboard_sr.qml \
    Keyboard_sr_email.qml \
    Keyboard_sr_url.qml \
    Keyboard_sr_url_search.qml

