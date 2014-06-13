TEMPLATE = lib
QT += qml quick
CONFIG += qt plugin no_keywords
TARGET = ubuntu-keyboard-qml

HEADERS += \
    plugin.h \
    inputmethodextensions.h \

SOURCES += \
    plugin.cpp \
    inputmethodextensions.cpp \

QMLDIR_FILE = qmldir

# deployment rules for the plugin
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)Ubuntu/Keyboard
target.path = $$installPath

qmldir_file.path = $$installPath
qmldir_file.files = $$QMLDIR_FILE

INSTALLS += target qmldir_file

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive


