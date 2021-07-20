TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TARGET = dummy
TEMPLATE = lib

lang_ja.path = "$$LOMIRI_KEYBOARD_PLUGIN_DIR/ja/"
lang_ja.files = *.qml *.js

lang_ja_keys.path = "$$LOMIRI_KEYBOARD_PLUGIN_DIR/ja/keys"
lang_ja_keys.files = keys/*.qml keys/*.js

INSTALLS += lang_ja lang_ja_keys

# for QtCreator
OTHER_FILES += \
    Keyboard_ja.qml \
    Keyboard_ja_email.qml \
    Keyboard_ja_url.qml \
    Keyboard_ja_url_search.qml \
    keys/CommitKey.qml \
    keys/CursorKey.qml \
    keys/FlickArea.qml \
    keys/FlickCharKey.qml \
    keys/FlickPop.qml \
    keys/FlickPopKey.qml \
    keys/KanaSwitchKey.qml \
    keys/ModifierKey.qml \
    keys/UndoKey.qml
