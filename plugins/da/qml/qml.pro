TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_da.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/da/"
lang_da.files = *.qml *.js

INSTALLS += lang_da

# for QtCreator
OTHER_FILES += \
    Keyboard_da.qml \
    Keyboard_da_email.qml \
    Keyboard_da_url.qml \
    Keyboard_da_url_search.qml

