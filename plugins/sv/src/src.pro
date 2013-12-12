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
    swedishplugin.h

TARGET          = $$qtLibraryTarget(swedishplugin)

EXAMPLE_FILES = swedishplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/sv/

lang_db_sv.path = $$PLUGIN_INSTALL_PATH
lang_db_sv.commands += \
  rm -f $$PWD/database_sv.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_sv.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_sv.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_sv.db $$PWD/free_ebook.txt

lang_db_sv.files += $$PWD/database_sv.db
QMAKE_EXTRA_TARGETS += lang_db_sv

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_sv

OTHER_FILES += \
    swedishplugin.json \
    free_ebook.txt

LIBS += $${TOP_SRCDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
