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
    bulgarianplugin.h

TARGET          = $$qtLibraryTarget(bgplugin)

EXAMPLE_FILES = bulgarianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/bg/

lang_db_bg.commands += \
  rm -f $$PWD/database_bg.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_bg.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_bg.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_bg.db $$PWD/free_ebook.txt
lang_db_bg.files += $$PWD/database_bg.db

lang_db_bg_install.files += $$PWD/database_bg.db
lang_db_bg_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_bg lang_db_bg_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_bg_install

OTHER_FILES += \
    bulgarianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
