TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/pt/"
lang.files = languages/pt/*.qml languages/pt/*.js

INSTALLS +=     lang

# for QtCreator
OTHER_FILES += \
    Keyboard_pt.qml \
    Keyboard_pt_email.qml \
    Keyboard_pt_url.qml \
    Keyboard_pt_url_search.qml

