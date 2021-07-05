TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_nb.path = "$${LOMIRI_KEYBOARD_LIB_DIR}/nb/"
lang_nb.files = *.qml *.js

INSTALLS += lang_nb

# for QtCreator
OTHER_FILES += \
    Keyboard_nb.qml \
    Keyboard_nb_email.qml \
    Keyboard_nb_url.qml \
    Keyboard_nb_url_search.qml

