TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_fr-ch.path = "$$UBUNTU_KEYBOARD_LIB_DIR/fr-ch/"
lang_fr-ch.files = *.qml *.js

INSTALLS += lang_fr-ch

# for QtCreator
OTHER_FILES += \
    Keyboard_fr-ch.qml \
    Keyboard_fr-ch_email.qml \
    Keyboard_fr-ch_url.qml \
    Keyboard_fr-ch_url_search.qml

