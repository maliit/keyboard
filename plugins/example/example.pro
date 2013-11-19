
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../../src/
HEADERS         = exampleplugin.h \
    exampleplugin.h
SOURCES         = exampleplugin.cpp
TARGET          = $$qtLibraryTarget(exampleplugin)
DESTDIR         = ../plugins

# EXAMPLE_FILES = exampleplugin.json

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/exampleplugin/plugin
INSTALLS += target

OTHER_FILES += \
    exampleplugin.json
