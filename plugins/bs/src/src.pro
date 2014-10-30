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
    bosnianplugin.h

TARGET          = $$qtLibraryTarget(bosnianplugin)

EXAMPLE_FILES = bosnianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/bs/

lang_db_bs.commands += \
  rm -f $$PWD/database_bs.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_bs.db $$PWD/u-registraturi.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_bs.db $$PWD/u-registraturi.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_bs.db $$PWD/u-registraturi.txt
lang_db_bs.files += $$PWD/database_bs.db

lang_db_bs_install.files += $$PWD/database_bs.db
lang_db_bs_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_bs lang_db_bs_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_bs_install

OTHER_FILES += \
    bosnianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
