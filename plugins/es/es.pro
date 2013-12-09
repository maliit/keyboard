
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/es/

CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    src \
    qml

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
