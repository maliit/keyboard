
TARGET = dummy
TEMPLATE = lib

lang.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/fr"
lang.files = languages/fr/*.qml languages/fr/*.js

INSTALLS +=     lang

# for QtCreator
OTHER_FILES += \
    Keyboard_fr.qml \
    Keyboard_fr_email.qml \
    Keyboard_fr_url.qml \
    Keyboard_fr_url_search.qml

