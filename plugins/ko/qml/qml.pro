TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_ko.path = "$$UBUNTU_KEYBOARD_LIB_DIR/ko/"
lang_ko.files = *.qml *.js

INSTALLS += lang_ko

# for QtCreator
OTHER_FILES += \
    Keyboard_ko.qml \
    Keyboard_ko_email.qml \
    Keyboard_ko_url.qml \
    Keyboard_ko_url_search.qml

