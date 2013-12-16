TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_hu.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/hu/"
lang_hu.files = *.qml *.js

INSTALLS += lang_hu

# for QtCreator
OTHER_FILES += \
    Keyboard_hu.qml \
    Keyboard_hu_email.qml \
    Keyboard_hu_url.qml \
    Keyboard_hu_url_search.qml

