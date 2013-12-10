
TARGET = dummy
TEMPLATE = lib

lang_en.path = "$$UBUNTU_KEYBOARD_LIB_DIR/en/"
lang_en.files = *.qml *.js

INSTALLS += lang_en

# for QtCreator
OTHER_FILES += \
    Keyboard_en.qml \
    Keyboard_en_email.qml \
    Keyboard_en_url.qml \
    Keyboard_en_url_search.qml

