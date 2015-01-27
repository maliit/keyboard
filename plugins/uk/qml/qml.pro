TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_uk.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/uk/"
lang_uk.files = *.qml *.js

INSTALLS += lang_uk

# for QtCreator
OTHER_FILES += \
    Keyboard_uk.qml \
    Keyboard_uk_email.qml \
    Keyboard_uk_url.qml \
    Keyboard_uk_url_search.qml

