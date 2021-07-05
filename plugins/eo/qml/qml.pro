TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_eo.path = "$$LOMIRI_KEYBOARD_LIB_DIR/eo/"
lang_eo.files = *.qml *.js

INSTALLS += lang_eo

# for QtCreator
OTHER_FILES += \
    Keyboard_eo.qml \
    Keyboard_eo_email.qml \
    Keyboard_eo_url.qml \
    Keyboard_eo_url_search.qml
