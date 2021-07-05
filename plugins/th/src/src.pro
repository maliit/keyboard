TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += \
    $${TOP_SRCDIR}/src/ \
    $${TOP_SRCDIR}/src/lib/logic

HEADERS         = \
                  thaiplugin.h \
                  thailanguagefeatures.h \
                  $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.h

SOURCES         = \
                  thaiplugin.cpp \
                  thailanguagefeatures.cpp \
                  $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.cpp

TARGET          = $$qtLibraryTarget(thplugin)

PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/th/

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target

OTHER_FILES += \
    thaiplugin.json
