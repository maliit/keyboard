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
    gaelicplugin.h

TARGET          = $$qtLibraryTarget(gaelicplugin)

EXAMPLE_FILES = gaelicplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/gd/

lang_db_gd.commands += \
  rm -f $$PWD/database_gd.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_gd.db $$PWD/teacsa.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_gd.db $$PWD/teacsa.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_gd.db $$PWD/teacsa.txt
lang_db_gd.files += $$PWD/database_gd.db

lang_db_gd_install.files += $$PWD/database_gd.db
lang_db_gd_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_gd lang_db_gd_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_gd_install


OTHER_FILES += \
    gaelicplugin.json \
    teacsa.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
