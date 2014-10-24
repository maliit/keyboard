TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_bs.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/bs/"
lang_bs.files = *.qml *.js

INSTALLS += lang_bs

# for QtCreator
OTHER_FILES += \
    Keyboard_bs.qml \
    Keyboard_bs_email.qml \
    Keyboard_bs_url.qml \
    Keyboard_bs_url_search.qml

