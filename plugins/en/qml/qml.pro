
TARGET = dummy
TEMPLATE = lib

lang_en.path = "$$PLUGIN_INSTALL_PATH"
lang_en.files = languages/en/*.qml languages/en/*.js

INSTALLS +=     lang_en

# for QtCreator
OTHER_FILES += \
    Keyboard_en.qml \
    Keyboard_en_email.qml \
    Keyboard_en_url.qml \
    Keyboard_en_url_search.qml

