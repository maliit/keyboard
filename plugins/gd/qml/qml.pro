TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_gd.path = "$$LOMIRI_KEYBOARD_LIB_DIR/gd/"
lang_gd.files = *.qml *.js

INSTALLS += lang_gd

# for QtCreator
OTHER_FILES += \
    Keyboard_gd.qml \
    Keyboard_gd_email.qml \
    Keyboard_gd_url.qml \
    Keyboard_gd_url_search.qml

