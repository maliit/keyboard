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
    italianplugin.h

TARGET          = $$qtLibraryTarget(italianplugin)

EXAMPLE_FILES = italianplugin.json

# generate database for presage:
QMAKE_POST_LINK = text2ngram -n 1 -l -f sqlite -o $$TOP_BUILDDIR/database_it.db $$PWD/la_francia_dal_primo_impero.txt
QMAKE_CLEAN     += $$TOP_BUILDDIR/database_it.db

# install
lang_db_it.path = $${UBUNTU_KEYBOARD_LIB_DIR}
lang_db_it.files += $$TOP_BUILDDIR/database_it.db

target.path = $${UBUNTU_KEYBOARD_LIB_DIR}
INSTALLS += target lang_db_it

OTHER_FILES += \
    italianplugin.json

LIBS += -L$${TOP_SRCDIR}/plugins/plugins/ -lwesternsupport

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
