
TARGET = dummy
TEMPLATE = lib

lang.path = "$$PLUGIN_INSTALL_PATH"
lang.files = languages/de/*.qml languages/de/*.js

INSTALLS +=     lang

# for QtCreator
OTHER_FILES += \
    Keyboard_de.qml \
    Keyboard_de_email.qml \
    Keyboard_de_url.qml \
    Keyboard_de_url_search.qml

