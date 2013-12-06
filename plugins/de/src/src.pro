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
    germanplugin.h

TARGET          = $$qtLibraryTarget(germanplugin)

EXAMPLE_FILES = germanplugin.json

# generate database for presage:
QMAKE_CLEAN     += $$TOP_BUILDDIR/database_de.db

# install
lang_db_de.path = $${UBUNTU_KEYBOARD_LIB_DIR}
lang_db_de.commands += \
  rm -f $$PWD/database_de.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_de.db $$PWD/buddenbrooks.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_de.db $$PWD/buddenbrooks.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_de.db $$PWD/buddenbrooks.txt

lang_db_de.files += $$PWD/database_de.db
QMAKE_EXTRA_TARGETS += lang_db_de

target.path = $${UBUNTU_KEYBOARD_LIB_DIR}
INSTALLS += target lang_db_de


OTHER_FILES += \
    germanplugin.json \
    buddenbrooks.txt

LIBS += $${TOP_SRCDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
