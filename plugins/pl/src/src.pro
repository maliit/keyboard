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
    polishplugin.h

TARGET          = $$qtLibraryTarget(polishplugin)

EXAMPLE_FILES = polishplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/pl/

lang_db_pl.path = $$PLUGIN_INSTALL_PATH
lang_db_pl.commands += \
  rm -f $$PWD/database_pl.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_pl.db $$PWD/ziemia_obiecana_tom_pierwszy_4.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_pl.db $$PWD/ziemia_obiecana_tom_pierwszy_4.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_pl.db $$PWD/ziemia_obiecana_tom_pierwszy_4.txt

lang_db_pl.files += $$PWD/database_pl.db
QMAKE_EXTRA_TARGETS += lang_db_pl

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_pl

OTHER_FILES += \
    polishplugin.json \
    ziemia_obiecana_tom_pierwszy_4.txt

LIBS += $${TOP_SRCDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
