
TARGET = dummy
TEMPLATE = lib

lang.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/es/"
lang.files = languages/es/*.qml languages/es/*.js

INSTALLS +=     lang

# for QtCreator
OTHER_FILES += \
    Keyboard_es.qml \
    Keyboard_es_email.qml \
    Keyboard_es_url.qml \
    Keyboard_es_url_search.qml

