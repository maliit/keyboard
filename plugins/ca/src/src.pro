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
    catalanplugin.h

TARGET          = $$qtLibraryTarget(caplugin)

EXAMPLE_FILES = catalanplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/ca/

lang_db_ca.commands += \
  rm -f $$PWD/database_ca.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_ca.db $$PWD/paulina_buxareu.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_ca.db $$PWD/paulina_buxareu.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_ca.db $$PWD/paulina_buxareu.txt
lang_db_ca.files += $$PWD/database_ca.db

lang_db_ca_install.files += $$PWD/database_ca.db
lang_db_ca_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_ca lang_db_ca_install

overrides.files += $$PWD/overrides.csv
overrides.path += $$PLUGIN_INSTALL_PATH

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_ca_install overrides

OTHER_FILES += \
    catalanplugin.json \
    paulina_buxareu.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
