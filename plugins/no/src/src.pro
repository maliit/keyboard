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
    norwegianplugin.h

TARGET          = $$qtLibraryTarget(norwegianplugin)

EXAMPLE_FILES = norwegianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/no/

lang_db_no.commands += \
  rm -f $$PWD/database_no.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_no.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_no.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_no.db $$PWD/free_ebook.txt
lang_db_no.files += $$PWD/database_no.db

lang_db_no_install.files += $$PWD/database_no.db
lang_db_no_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_no lang_db_no_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_no_install

OTHER_FILES += \
    norwegianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
