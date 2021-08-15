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
    croatianplugin.h

TARGET          = $$qtLibraryTarget(hrplugin)

EXAMPLE_FILES = croatianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/hr/

lang_db_hr.commands += \
  rm -f $$PWD/database_hr.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_hr.db $$PWD/knjiga.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_hr.db $$PWD/knjiga.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_hr.db $$PWD/knjiga.txt
lang_db_hr.files += $$PWD/database_hr.db

lang_db_hr_install.files += $$PWD/database_hr.db
lang_db_hr_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_hr lang_db_hr_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_hr_install

OTHER_FILES += \
    croatianplugin.json \
    free_ebook.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
