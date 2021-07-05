TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_tr.path = "$$LOMIRI_KEYBOARD_PLUGIN_DIR/tr/"
lang_tr.files = *.qml *.js

INSTALLS += lang_tr

# for QtCreator
OTHER_FILES += \
    Keyboard_tr.qml \
    Keyboard_tr_email.qml \
    Keyboard_tr_url.qml \
    Keyboard_tr_url_search.qml

