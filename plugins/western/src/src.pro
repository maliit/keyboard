
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += \
    ../../src/ \
    ../../src/lib/logic

HEADERS         = \
    westernplugin.h \
    candidatescallback.h \
    spellchecker.h \
    languagefeatures.h
SOURCES         = \
    westernplugin.cpp \
    candidatescallback.cpp \
    spellchecker.cpp \
    languagefeatures.cpp

TARGET          = $$qtLibraryTarget(westernplugin)
DESTDIR         = ../plugins

EXAMPLE_FILES = westernplugin.json

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/westernplugin/plugin
INSTALLS += target

OTHER_FILES += \
    westernplugin.json

LIBS += -lpresage
DEFINES += HUNSPELL_DICT_PATH=\\\"$$HUNSPELL_DICT_PATH\\\"
