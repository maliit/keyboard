TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_emoji.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/emoji/"
lang_emoji.files = *.qml *.js

INSTALLS += lang_emoji

# for QtCreator
OTHER_FILES += \
    Keyboard_emoji.qml \
    Keyboard_emoji_email.qml \
    Keyboard_emoji_url.qml \
    Keyboard_emoji_url_search.qml

