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
images.files = images/*.png

INSTALLS += qml qml_keys qml_languages images

# for QtCreator
OTHER_FILES += \
    maliit-keyboard.qml \
    maliit-keyboard-extended.qml \
    maliit-magnifier.qml \
    Keyboard.qml \
    maliit-ui-constants.qml \
    RotationHelper.qml \
    WordRibbon.qml \
    Popper.qml \
    KeyboardContainer.qml \
    languages/Keyboard_symbols.qml \
    languages/Keyboard_en_us.qml \
    keys/key_constants.js \
    keys/KeyPad.qml \
    keys/CharKey.qml \
    keys/ActionKey.qml \
    keys/BackspaceKey.qml \
    keys/ShiftKey.qml \
    keys/SymbolShiftKey.qml \
    keys/ReturnKey.qml \
    keys/Popper.qml \
    keys/OneTwoKey.qml \
    languages/Keyboard_en_url.qml \
    languages/Keyboard_en_url_search.qml \
    languages/Keyboard_en_email.qml \
    languages/Keyboard_numbers.qml \
    languages/Keyboard_telephone.qml \
    keys/ExtendedKeysSelector.qml \
    keys/SpaceKey.qml

