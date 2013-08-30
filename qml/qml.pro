include(../config.pri)

TARGET = dummy
TEMPLATE = lib

qml.path = $$UBUNTU_KEYBOARD_DATA_DIR
qml.files = *.qml

INSTALLS += qml
OTHER_FILES += \
    maliit-keyboard.qml \
    maliit-keyboard-extended.qml \
    maliit-magnifier.qml \
    Keyboard.qml \
    maliit-ui-constants.qml \
    RotationHelper.qml \
    WordRibbon.qml \
    Popper.qml \
    prototype.qml \
    KeyPad.qml \
    Keyboard_symbols.qml \
    Keyboard_en_us.qml \
    constants.js \
    keys/CharKey.qml \
    keys/ActionKey.qml \
    keys/BackspaceKey.qml \
    keys/EnterKey.qml \
    keys/ShiftKey.qml \
    keys/SymbolShiftKey.qml \


