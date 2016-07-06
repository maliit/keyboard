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
    latvianplugin.h

TARGET          = $$qtLibraryTarget(lvplugin)

EXAMPLE_FILES = latvianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/lv/

lang_db_lv.commands += \
  rm -f $$PWD/database_lv.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_lv.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_lv.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_lv.db $$PWD/free_ebook.txt
lang_db_lv.files += $$PWD/database_lv.db
lang_db_lv_install.path = $$PLUGIN_INSTALL_PATH
lang_db_lv_install.files += $$PWD/database_lv.db

QMAKE_EXTRA_TARGETS += lang_db_lv lang_db_lv_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_lv_install

OTHER_FILES += \
    latvianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
