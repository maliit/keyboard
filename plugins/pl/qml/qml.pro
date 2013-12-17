TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_pl.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/pl/"
lang_pl.files = *.qml *.js

INSTALLS += lang_pl

# for QtCreator
OTHER_FILES += \
    Keyboard_pl.qml \
    Keyboard_pl_email.qml \
    Keyboard_pl_url.qml \
    Keyboard_pl_url_search.qml

