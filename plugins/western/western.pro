
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../../src/
HEADERS         = westernplugin.h \
    candidatescallback.h
SOURCES         = westernplugin.cpp \ 
    candidatescallback.cpp
TARGET          = $$qtLibraryTarget(westernplugin)
DESTDIR         = ../plugins

EXAMPLE_FILES = westernplugin.json

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/westernplugin/plugin
INSTALLS += target

OTHER_FILES += \
    westernplugin.json

LIBS += -lpresage
