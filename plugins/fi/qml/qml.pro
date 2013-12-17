TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_fi.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/fi/"
lang_fi.files = *.qml *.js

INSTALLS += lang_fi

# for QtCreator
OTHER_FILES += \
    Keyboard_fi.qml \
    Keyboard_fi_email.qml \
    Keyboard_fi_url.qml \
    Keyboard_fi_url_search.qml

