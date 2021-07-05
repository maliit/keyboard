TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_is.path = "$${LOMIRI_KEYBOARD_PLUGIN_DIR}/is/"
lang_is.files = *.qml *.js

INSTALLS += lang_is

# for QtCreator
OTHER_FILES += \
    Keyboard_is.qml \
    Keyboard_is_email.qml \
    Keyboard_is_url.qml \
    Keyboard_is_url_search.qml

