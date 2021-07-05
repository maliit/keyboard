TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_lt.path = "$${LOMIRI_KEYBOARD_LIB_DIR}/lt/"
lang_lt.files = *.qml *.js

INSTALLS += lang_lt

# for QtCreator
OTHER_FILES += \
    Keyboard_lt.qml \
    Keyboard_lt_email.qml \
    Keyboard_lt_url.qml \
    Keyboard_lt_url_search.qml

