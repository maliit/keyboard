
TARGET = dummy
TEMPLATE = lib

lang.path = "$$PLUGIN_INSTALL_PATH"
lang.files = languages/pt/*.qml languages/pt/*.js

INSTALLS +=     lang

# for QtCreator
OTHER_FILES += \
    Keyboard_pt.qml \
    Keyboard_pt_email.qml \
    Keyboard_pt_url.qml \
    Keyboard_pt_url_search.qml

