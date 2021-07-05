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
    lithuanianplugin.h

TARGET          = $$qtLibraryTarget(ltplugin)

EXAMPLE_FILES = lithuanianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_LIB_DIR}/lt/

lang_db_lt.commands += \
  rm -f $$PWD/database_lt.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_lt.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_lt.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_lt.db $$PWD/free_ebook.txt
lang_db_lt.files += $$PWD/database_lt.db
lang_db_lt_install.path = $$PLUGIN_INSTALL_PATH
lang_db_lt_install.files += $$PWD/database_lt.db

QMAKE_EXTRA_TARGETS += lang_db_lt lang_db_lt_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_lt_install

OTHER_FILES += \
    lithuanianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
