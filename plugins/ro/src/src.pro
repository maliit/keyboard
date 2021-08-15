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
    romanianplugin.h

TARGET          = $$qtLibraryTarget(roplugin)

EXAMPLE_FILES = romanianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/ro/

lang_db_ro.commands += \
  rm -f $$PWD/database_ro.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_ro.db $$PWD/amintiri_din_copilarie.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_ro.db $$PWD/amintiri_din_copilarie.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_ro.db $$PWD/amintiri_din_copilarie.txt
lang_db_ro.files += $$PWD/database_ro.db

lang_db_ro_install.files += $$PWD/database_ro.db
lang_db_ro_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_ro lang_db_ro_install

overrides.files += $$PWD/overrides.csv
overrides.path += $$PLUGIN_INSTALL_PATH

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_ro_install overrides

OTHER_FILES += \
    romanianplugin.json \
    amintiri_din_copilarie.txt

PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
