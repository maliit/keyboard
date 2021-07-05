TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_testlayout.path = "$${LOMIRI_KEYBOARD_TEST_DIR}/testlayout/"
lang_testlayout.files = *.qml *.js

INSTALLS += lang_testlayout

# for QtCreator
OTHER_FILES += \
    Keyboard_testlayout.qml \
    Keyboard_testlayout_email.qml \
    Keyboard_testlayout_url.qml \
    Keyboard_testlayout_url_search.qml

