TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_sl.path = "$${LOMIRI_KEYBOARD_PLUGIN_DIR}/sl/"
lang_sl.files = *.qml *.js

INSTALLS += lang_sl

# for QtCreator
OTHER_FILES += \
    Keyboard_sl.qml \
    Keyboard_sl_email.qml \
    Keyboard_sl_url.qml \
    Keyboard_sl_url_search.qml

