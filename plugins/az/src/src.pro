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
    azerbaijaniplugin.h

TARGET          = $$qtLibraryTarget(azplugin)

EXAMPLE_FILES = azerbaijaniplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_LIB_DIR}/az/

lang_db_az.commands += \
  rm -f $$PWD/database_az.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_az.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_az.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_az.db $$PWD/free_ebook.txt
lang_db_az.files += $$PWD/database_az.db

lang_db_az_install.files += $$PWD/database_az.db
lang_db_az_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_az lang_db_az_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_az_install

OTHER_FILES += \
    free_ebook.txt \
    azerbaijaniplugin.json

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
