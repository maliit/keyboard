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
    italianplugin.h

TARGET          = $$qtLibraryTarget(italianplugin)

EXAMPLE_FILES = italianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/it/

lang_db_it.commands += \
  rm -f $$PWD/database_it.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_it.db $$PWD/la_francia_dal_primo_impero.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_it.db $$PWD/la_francia_dal_primo_impero.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_it.db $$PWD/la_francia_dal_primo_impero.txt
lang_db_it.files += $$PWD/database_it.db

lang_db_it_install.files += $$PWD/database_it.db
lang_db_it_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_it lang_db_it_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_it_install

OTHER_FILES += \
    italianplugin.json \
    la_francia_dal_primo_impero.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
