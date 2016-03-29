TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += \
    $${TOP_SRCDIR}/src/ \
    $${TOP_SRCDIR}/src/lib/ \
    $${TOP_SRCDIR}/src/lib/logic/

HEADERS         = \
    japaneseplugin.h \
    japaneselanguagefeatures.h \
    anthyadapter.h \
    $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.h

SOURCES         = \
    japaneseplugin.cpp \
    japaneselanguagefeatures.cpp \
    anthyadapter.cpp \
    $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.cpp

TARGET          = $$qtLibraryTarget(japlugin)

EXAMPLE_FILES = japaneseplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/ja/

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target

OTHER_FILES += \
    jaglishplugin.json

LIBS += -lpresage -lanthy -lanthydic
