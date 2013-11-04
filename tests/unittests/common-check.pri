QMAKE_EXTRA_TARGETS += check
check.target = check

# This enables the maliit library for C++ code
CONFIG += maliit-plugins

# Note that if datafiles are generated they would end out in
# $$OUT_PWD and this would fail
DATA_DIR = $$PWD/../../data
PLUGIN_BUILD_DIR = $$OUT_PWD/../../../src/plugin

check.commands = \
    MALIIT_PLUGINS_DATADIR=$$DATA_DIR \
    MALIIT_KEYBOARD_DATADIR=$$DATA_DIR \
    LD_LIBRARY_PATH=$$PLUGIN_BUILD_DIR:$$MALIIT_INSTALL_LIBS:$$[QT_INSTALL_LIBS]:$(LD_LIBRARY_PATH)

check.depends += $$TARGET

LIBS += ../common/$$maliitStaticLib(tests-common)
POST_TARGETDEPS += ../common/$$maliitStaticLib(tests-common)

INCLUDEPATH += \
    ../ \
    ../common \
    ../../../src/lib \
    ../../../src \
