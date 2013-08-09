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
    Popper.qml

