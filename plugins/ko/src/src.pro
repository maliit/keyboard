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
    koreanplugin.h \
    koreanlanguagefeatures.h \
    $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.h

SOURCES         = \
    koreanplugin.cpp \
    koreanlanguagefeatures.cpp \
    $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.cpp

TARGET          = $$qtLibraryTarget(koplugin)

EXAMPLE_FILES = koreanplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/ko/

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target

OTHER_FILES += \
    koreanplugin.json

#LIBS += 
