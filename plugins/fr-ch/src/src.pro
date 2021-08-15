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
    $${TOP_SRCDIR}/plugins/westernsupport

HEADERS         = \
    frenchplugin.h

TARGET          = $$qtLibraryTarget(fr-chplugin)

EXAMPLE_FILES = frenchplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/fr-ch/

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target

OTHER_FILES += \
    frenchplugin.json \

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
