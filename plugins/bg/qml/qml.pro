TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_bg.path = "$$LOMIRI_KEYBOARD_LIB_DIR/bg/"
lang_bg.files = *.qml *.js

INSTALLS += lang_bg

# for QtCreator
OTHER_FILES += \
    Keyboard_bg.qml \
    Keyboard_bg_email.qml \
    Keyboard_bg_url.qml \
    Keyboard_bg_url_search.qml
