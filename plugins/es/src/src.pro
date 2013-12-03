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
    spanishplugin.h

TARGET          = $$qtLibraryTarget(spanishplugin)

EXAMPLE_FILES = spanishplugin.json

# generate database for presage:
QMAKE_POST_LINK = text2ngram -n 1 -l -f sqlite -o $$TOP_BUILDDIR/database_es.db $$PWD/el_quijote.txt
QMAKE_POST_LINK = text2ngram -n 2 -l -f sqlite -o $$TOP_BUILDDIR/database_es.db $$PWD/el_quijote.txt
QMAKE_POST_LINK = text2ngram -n 3 -l -f sqlite -o $$TOP_BUILDDIR/database_es.db $$PWD/el_quijote.txt
QMAKE_CLEAN     += $$TOP_BUILDDIR/database_es.db

# install
lang_db_es.path = $${UBUNTU_KEYBOARD_LIB_DIR}
lang_db_es.files += $$TOP_BUILDDIR/database_es.db

target.path = $${UBUNTU_KEYBOARD_LIB_DIR}
INSTALLS += target lang_db_es

OTHER_FILES += \
    spanishplugin.json

LIBS += $${TOP_SRCDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
