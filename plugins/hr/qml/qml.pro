TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_hr.path = "$${LOMIRI_KEYBOARD_LIB_DIR}/hr/"
lang_hr.files = *.qml *.js

INSTALLS += lang_hr

# for QtCreator
OTHER_FILES += \
    Keyboard_hr.qml \
    Keyboard_hr_email.qml \
    Keyboard_hr_url.qml \
    Keyboard_hr_url_search.qml

