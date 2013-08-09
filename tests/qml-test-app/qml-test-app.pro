TARGET = dummy
TEMPLATE = lib

qml.path = $$UBUNTU_KEYBOARD_DATA_DIR/testapp
qml.files = *.qml

INSTALLS += qml
OTHER_FILES += \
    keyboard-test.qml \
    layout-test.qml 

