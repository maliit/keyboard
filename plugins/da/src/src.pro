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
    danishplugin.h

TARGET          = $$qtLibraryTarget(danishplugin)

EXAMPLE_FILES = danishplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/da/

lang_db_da.commands += \
  rm -f $$PWD/database_da.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_da.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_da.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_da.db $$PWD/free_ebook.txt
lang_db_da.files += $$PWD/database_da.db

lang_db_da_install.files += $$PWD/database_da.db
lang_db_da_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_da lang_db_da_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_da_install

OTHER_FILES += \
    danishplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
