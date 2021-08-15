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
    icelandicplugin.h

TARGET          = $$qtLibraryTarget(isplugin)

EXAMPLE_FILES = icelandicplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/is/

lang_db_is.commands += \
  rm -f $$PWD/database_is.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_is.db $$PWD/althingi_umraedur_2004_2005.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_is.db $$PWD/althingi_umraedur_2004_2005.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_is.db $$PWD/althingi_umraedur_2004_2005.txt
lang_db_is.files += $$PWD/database_is.db

lang_db_is_install.path = $$PLUGIN_INSTALL_PATH
lang_db_is_install.files += $$PWD/database_is.db

QMAKE_EXTRA_TARGETS += lang_db_is lang_db_is_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_is_install

OTHER_FILES += \
    icelandicplugin.json \
    althingi_umraedur_2004_2005.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
