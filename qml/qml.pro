include(../config.pri)

TARGET = dummy
TEMPLATE = lib

qml.path = $$UBUNTU_KEYBOARD_DATA_DIR
qml.files = *.qml *.js

qml_keys.path = "$$UBUNTU_KEYBOARD_DATA_DIR/keys"
qml_keys.files = keys/*.qml keys/*.js

qml_languages.path = "$$UBUNTU_KEYBOARD_DATA_DIR/languages"
qml_languages.files = languages/*.qml languages/*.js

images.path = "$$UBUNTU_KEYBOARD_DATA_DIR/images"
images.files = images/*.png images/*.sci

INSTALLS += qml qml_keys qml_languages images

# for QtCreator
OTHER_FILES += \
    maliit-keyboard.qml \
    maliit-keyboard-extended.qml \
    maliit-magnifier.qml \
    maliit-ui-constants.qml \
    Keyboard.qml \
    KeyboardContainer.qml \
    Popper.qml \
    WordRibbon.qml \
    keys/key_constants.js \
    keys/ActionKey.qml \
    keys/CharKey.qml \
    keys/BackspaceKey.qml \
    keys/ExtendedKeysSelector.qml \
    keys/KeyPad.qml \
    keys/OneTwoKey.qml \
    keys/Popper.qml \
    keys/PressArea.qml \
    keys/ReturnKey.qml \
    keys/ShiftKey.qml \
    keys/SpaceKey.qml \
    keys/Spacer.qml \
    keys/SymbolShiftKey.qml \
    keys/UrlKey.qml \
    languages/Keyboard_en_email.qml \
    languages/Keyboard_en_url.qml \
    languages/Keyboard_en_url_search.qml \
    languages/Keyboard_en_us.qml \
    languages/Keyboard_numbers.qml \
    languages/Keyboard_symbols.qml \
    languages/Keyboard_telephone.qml \
    languages/Keyboard_zh_cn_pinyin.qml \
