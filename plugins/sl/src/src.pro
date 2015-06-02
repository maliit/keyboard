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
    slovenianplugin.h

TARGET          = $$qtLibraryTarget(slplugin)

EXAMPLE_FILES = slovenianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/sl/

lang_db_sl.commands += \
  rm -f $$PWD/database_sl.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_sl.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_sl.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_sl.db $$PWD/free_ebook.txt
lang_db_sl.files += $$PWD/database_sl.db

lang_db_sl_install.files += $$PWD/database_sl.db
lang_db_sl_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_sl lang_db_sl_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_sl_install

OTHER_FILES += \
    slovenianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
