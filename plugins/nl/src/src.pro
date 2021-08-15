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

TARGET          = $$qtLibraryTarget(nlplugin)

EXAMPLE_FILES = dutchplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/nl/

lang_db_nl.commands += \
  rm -f $$PWD/database_nl.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_nl.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_nl.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_nl.db $$PWD/free_ebook.txt
lang_db_nl.files += $$PWD/database_nl.db

lang_db_nl_install.files += $$PWD/database_nl.db
lang_db_nl_install.path = $$PLUGIN_INSTALL_PATH

overrides.files += $$PWD/overrides.csv
overrides.path += $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_nl lang_db_nl_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_nl_install overrides

OTHER_FILES += \
    dutchplugin.json \
    free_ebook.txt

PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
