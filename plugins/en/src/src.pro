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
    englishplugin.h

TARGET          = $$qtLibraryTarget(englishplugin)

EXAMPLE_FILES = englishplugin.json

# generate database for presage:
QMAKE_POST_LINK = text2ngram -n 1 -l -f sqlite -o $$TOP_BUILDDIR/database_en.db $$PWD/the_picture_of_dorian_gray.txt
QMAKE_POST_LINK = text2ngram -n 2 -l -f sqlite -o $$TOP_BUILDDIR/database_en.db $$PWD/the_picture_of_dorian_gray.txt
QMAKE_POST_LINK = text2ngram -n 3 -l -f sqlite -o $$TOP_BUILDDIR/database_en.db $$PWD/the_picture_of_dorian_gray.txt
QMAKE_CLEAN     += $$TOP_BUILDDIR/database_en.db

# install
lang_db_en.path = $${UBUNTU_KEYBOARD_LIB_DIR}
lang_db_en.files += $$TOP_BUILDDIR/database_en.db

target.path = $${UBUNTU_KEYBOARD_LIB_DIR}
INSTALLS += target lang_db_en

OTHER_FILES += \
    englishplugin.json

LIBS += $${TOP_SRCDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
