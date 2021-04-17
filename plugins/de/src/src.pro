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
    germanplugin.h

TARGET          = $$qtLibraryTarget(deplugin)

EXAMPLE_FILES = germanplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/de/

lang_db_de.commands += \
  rm -f $$PWD/database_de.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_de.db $$PWD/buddenbrooks.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_de.db $$PWD/buddenbrooks.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_de.db $$PWD/buddenbrooks.txt
lang_db_de.files += $$PWD/database_de.db

lang_db_de_install.path = $$PLUGIN_INSTALL_PATH
lang_db_de_install.files += $$PWD/database_de.db

QMAKE_EXTRA_TARGETS += lang_db_de lang_db_de_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_de_install

OTHER_FILES += \
    germanplugin.json \
    buddenbrooks.txt

PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
