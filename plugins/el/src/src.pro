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
    greekplugin.h

TARGET          = $$qtLibraryTarget(elplugin)

EXAMPLE_FILES = greekplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/el/

lang_db_el.commands += \
  rm -f $$PWD/database_el.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_el.db $$PWD/grazia_deledda-christos_alexandridis.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_el.db $$PWD/grazia_deledda-christos_alexandridis.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_el.db $$PWD/grazia_deledda-christos_alexandridis.txt
lang_db_el.files += $$PWD/database_el.db

lang_db_el_install.files += $$PWD/database_el.db
lang_db_el_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_el lang_db_el_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_el_install

OTHER_FILES += \
    greekplugin.json \
    grazia_deledda-christos_alexandridis.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
