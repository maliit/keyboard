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
    czechplugin.h

TARGET          = $$qtLibraryTarget(csplugin)

EXAMPLE_FILES = czechplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/cs/

lang_db_cs.commands += \
  rm -f $$PWD/database_cs.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_cs.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_cs.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_cs.db $$PWD/free_ebook.txt
lang_db_cs.files += $$PWD/database_cs.db

lang_db_cs_install.path = $$PLUGIN_INSTALL_PATH
lang_db_cs_install.files += $$PWD/database_cs.db

QMAKE_EXTRA_TARGETS += lang_db_cs lang_db_cs_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_cs_install

OTHER_FILES += \
    czechplugin.json \
    free_ebook.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
