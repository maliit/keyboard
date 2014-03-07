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
    hebrewplugin.h

TARGET          = $$qtLibraryTarget(hebrewplugin)

EXAMPLE_FILES = hebrewplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/he/

lang_db_he.path = $$PLUGIN_INSTALL_PATH
lang_db_he.commands += \
  rm -f $$PWD/database_he.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_he.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_he.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_he.db $$PWD/free_ebook.txt

lang_db_he.files += $$PWD/database_he.db
QMAKE_EXTRA_TARGETS += lang_db_he

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_he

OTHER_FILES += \
    hebrewplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
