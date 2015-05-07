TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_sl.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/sl/"
lang_sl.files = *.qml *.js

INSTALLS += lang_hr

# for QtCreator
OTHER_FILES += \
    Keyboard_sl.qml \
    Keyboard_sl_email.qml \
    Keyboard_sl_url.qml \
    Keyboard_sl_url_search.qml

