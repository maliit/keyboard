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

TARGET          = $$qtLibraryTarget(esplugin)

EXAMPLE_FILES = spanishplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/es/

lang_db_es.commands += \
  rm -f $$PWD/database_es.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_es.db $$PWD/el_quijote.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_es.db $$PWD/el_quijote.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_es.db $$PWD/el_quijote.txt
lang_db_es.files += $$PWD/database_es.db

lang_db_es_install.files += $$PWD/database_es.db
lang_db_es_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_es lang_db_es_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_es_install

OTHER_FILES += \
    spanishplugin.json \
    el_quijote.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
