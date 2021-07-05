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
    russianplugin.h

TARGET          = $$qtLibraryTarget(ruplugin)

EXAMPLE_FILES = russianplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/ru/

lang_db_ru.commands += \
  rm -f $$PWD/database_ru.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_ru.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_ru.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_ru.db $$PWD/free_ebook.txt
lang_db_ru.files += $$PWD/database_ru.db

lang_db_ru_install.files += $$PWD/database_ru.db
lang_db_ru_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_ru lang_db_ru_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_ru_install

OTHER_FILES += \
    russianplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
