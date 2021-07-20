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
                  chewingadapter.h \
                  chewingplugin.h \
                  chewinglanguagefeatures.h \
                  $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.h

SOURCES         = \
                  chewingadapter.cpp \
                  chewingplugin.cpp \
                  chewinglanguagefeatures.cpp \
                  $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.cpp

TARGET          = $$qtLibraryTarget(zh-hantplugin)

EXAMPLE_FILES = chewingplugin.json

# install
target.path = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/zh-hant/
INSTALLS += target

OTHER_FILES += \
    chewingplugin.json

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
PKGCONFIG += chewing
