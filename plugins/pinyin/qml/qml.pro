TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_zh.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/zh/"
lang_zh.files = *.qml *.js

INSTALLS += lang_zh

# for QtCreator
OTHER_FILES += \
    Keyboard_zh_cn_pinyin.qml \
    Keyboard_zh_email.qml \
    Keyboard_zh_url.qml \
    Keyboard_zh_url_search.qml

