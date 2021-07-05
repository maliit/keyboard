TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_az.path = "$$LOMIRI_KEYBOARD_LIB_DIR/az/"
lang_az.files = *.qml *.js

INSTALLS += lang_az

# for QtCreator
OTHER_FILES += \
    Keyboard_az.qml \
    Keyboard_az_email.qml \
    Keyboard_az_url.qml \
    Keyboard_az_url_search.qml

