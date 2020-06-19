TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_th.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/th/"
lang_th.files = *.qml *.js

lang_th_keys.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/th/keys"
lang_th_keys.files = keys/*.qml keys/*.js

INSTALLS += lang_th lang_th_keys

# for QtCreator
OTHER_FILES += \
    Keyboard_th.qml \
    Keyboard_th_email.qml \
    Keyboard_th_url.qml \
    Keyboard_th_url_search.qml

