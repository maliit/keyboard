TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_ro.path = "$${LOMIRI_KEYBOARD_PLUGIN_DIR}/ro/"
lang_ro.files = *.qml *.js

INSTALLS += lang_ro

# for QtCreator
OTHER_FILES += \
    Keyboard_ro.qml \
    Keyboard_ro_email.qml \
    Keyboard_ro_url.qml \
    Keyboard_ro_url_search.qml

