TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += \
    $${TOP_SRCDIR}/src/ \
    $${TOP_SRCDIR}/src/lib/ \
    $${TOP_SRCDIR}/src/lib/logic/

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
DESTDIR         = $${TOP_BUILDDIR}/plugins/plugins

EXAMPLE_FILES = westernplugin.json

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/westernplugin/plugin
INSTALLS += target

OTHER_FILES += \
    westernplugin.json

LIBS += -lpresage
DEFINES += HUNSPELL_DICT_PATH=\\\"$$HUNSPELL_DICT_PATH\\\"
