TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_en@dv.path = "$$LOMIRI_KEYBOARD_PLUGIN_DIR/en@dv/"
lang_en@dv.files = *.qml *.js

INSTALLS += lang_en@dv

# for QtCreator
OTHER_FILES += \
    Keyboard_en@dv.qml \
    Keyboard_en@dv_email.qml \
    Keyboard_en@dv_url.qml \
    Keyboard_en@dv_url_search.qml

