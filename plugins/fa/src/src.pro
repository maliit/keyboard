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
    persianplugin.h

TARGET          = $$qtLibraryTarget(faplugin)

EXAMPLE_FILES = persianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/fa/

lang_db_fa.commands += \
  rm -f $$PWD/database_fa.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_fa.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_fa.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_fa.db $$PWD/free_ebook.txt
lang_db_fa.files += $$PWD/database_fa.db
lang_db_fa_install.path = $$PLUGIN_INSTALL_PATH
lang_db_fa_install.files += $$PWD/database_fa.db

QMAKE_EXTRA_TARGETS += lang_db_fa lang_db_fa_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_fa_install

OTHER_FILES += \
    persianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
