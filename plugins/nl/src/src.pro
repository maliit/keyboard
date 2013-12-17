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
    dutchplugin.h

TARGET          = $$qtLibraryTarget(dutchplugin)

EXAMPLE_FILES = dutchplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/nl/

lang_db_nl.path = $$PLUGIN_INSTALL_PATH
lang_db_nl.commands += \
  rm -f $$PWD/database_nl.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_nl.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_nl.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_nl.db $$PWD/free_ebook.txt

lang_db_nl.files += $$PWD/database_nl.db
QMAKE_EXTRA_TARGETS += lang_db_nl

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_nl

OTHER_FILES += \
    dutchplugin.json \
    free_ebook.txt

LIBS += $${TOP_SRCDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
