
TARGET = dummy
TEMPLATE = lib

lang.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages/it"
lang.files = languages/it/*.qml languages/it/*.js

INSTALLS +=     lang

# for QtCreator
OTHER_FILES += \
    Keyboard_it.qml \
    Keyboard_it_email.qml \
    Keyboard_it_url.qml \
    Keyboard_it_url_search.qml

