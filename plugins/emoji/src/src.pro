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
                  emojiplugin.h \
                  emojilanguagefeatures.h \
                  $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.h

SOURCES         = \
                  emojiplugin.cpp \
                  emojilanguagefeatures.cpp \
                  $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.cpp

TARGET          = $$qtLibraryTarget(emojiplugin)

EXAMPLE_FILES = emojiplugin.json

# install
target.path = $${LOMIRI_KEYBOARD_LIB_DIR}/emoji/
INSTALLS += target

OTHER_FILES += \
    emojiplugin.json
