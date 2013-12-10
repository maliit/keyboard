
TARGET = dummy
TEMPLATE = lib

lang_es.path = "$${UBUNTU_KEYBOARD_LIB_DIR}/es/"
lang_es.files = *.qml *.js

INSTALLS += lang_es

# for QtCreator
OTHER_FILES += \
    Keyboard_es.qml \
    Keyboard_es_email.qml \
    Keyboard_es_url.qml \
    Keyboard_es_url_search.qml

