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
    turkishplugin.h

TARGET          = $$qtLibraryTarget(trplugin)

EXAMPLE_FILES = turkishplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_LIB_DIR}/tr/

lang_db_tr.commands += \
  rm -f $$PWD/database_tr.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_tr.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_tr.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_tr.db $$PWD/free_ebook.txt
lang_db_tr.files += $$PWD/database_tr.db

lang_db_tr_install.files += $$PWD/database_tr.db
lang_db_tr_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_tr lang_db_tr_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_tr_install

OTHER_FILES += \
    free_ebook.txt \
    turkishplugin.json

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
