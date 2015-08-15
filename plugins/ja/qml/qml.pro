TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_ja.path = "$$UBUNTU_KEYBOARD_LIB_DIR/ja/"
lang_ja.files = *.qml *.js

INSTALLS += lang_ja

# for QtCreator
OTHER_FILES += \
    Keyboard_ja.qml \
    Keyboard_ja_email.qml \
    Keyboard_ja_url.qml \
    Keyboard_ja_url_search.qml

