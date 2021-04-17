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
    serbianplugin.h

TARGET          = $$qtLibraryTarget(srplugin)

EXAMPLE_FILES = serbianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/sr/

lang_db_sr.commands += \
  rm -f $$PWD/database_sr.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_sr.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_sr.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_sr.db $$PWD/free_ebook.txt
lang_db_sr.files += $$PWD/database_sr.db

lang_db_sr_install.files += $$PWD/database_sr.db
lang_db_sr_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_sr lang_db_sr_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_sr_install

OTHER_FILES += \
    serbianplugin.json \
    free_ebook.txt

PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
