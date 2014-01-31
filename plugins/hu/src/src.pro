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
    hungarianplugin.h

TARGET          = $$qtLibraryTarget(hungarianplugin)

EXAMPLE_FILES = hungarianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/hu/

lang_db_hu.path = $$PLUGIN_INSTALL_PATH
lang_db_hu.commands += \
  rm -f $$PWD/database_hu.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_hu.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_hu.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_hu.db $$PWD/free_ebook.txt

lang_db_hu.files += $$PWD/database_hu.db
QMAKE_EXTRA_TARGETS += lang_db_hu

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_hu

OTHER_FILES += \
    hungarianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
