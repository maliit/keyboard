TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_lv.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/lv/"
lang_lv.files = *.qml *.js

INSTALLS += lang_lv

# for QtCreator
OTHER_FILES += \
    Keyboard_lv.qml \
    Keyboard_lv_email.qml \
    Keyboard_lv_url.qml \
    Keyboard_lv_url_search.qml

