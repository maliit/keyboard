
TARGET = dummy
TEMPLATE = lib

lang.path = "$$PLUGIN_INSTALL_PATH"
lang.files = languages/es/*.qml languages/es/*.js

INSTALLS +=     lang

# for QtCreator
OTHER_FILES += \
    Keyboard_es.qml \
    Keyboard_es_email.qml \
    Keyboard_es_url.qml \
    Keyboard_es_url_search.qml

