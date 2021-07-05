TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_fa.path = "$${LOMIRI_KEYBOARD_PLUGIN_DIR}/fa/"
lang_fa.files = *.qml *.js

INSTALLS += lang_fa

# for QtCreator
OTHER_FILES += \
    Keyboard_fa.qml \
    Keyboard_fa_email.qml \
    Keyboard_fa_url.qml \
    Keyboard_fa_url_search.qml \
    Keyboard_symbols_fa.qml

