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
    englishplugin.h

TARGET          = $$qtLibraryTarget(enplugin)

EXAMPLE_FILES = englishplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/en/

lang_db_en.commands += \
  rm -f $$PWD/database_en.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_en.db $$PWD/the_picture_of_dorian_gray.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_en.db $$PWD/the_picture_of_dorian_gray.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_en.db $$PWD/the_picture_of_dorian_gray.txt
lang_db_en.files += $$PWD/database_en.db

lang_db_en_install.files += $$PWD/database_en.db
lang_db_en_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_en lang_db_en_install

overrides.files += $$PWD/overrides.csv
overrides.path += $$PLUGIN_INSTALL_PATH

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_en_install overrides

OTHER_FILES += \
    englishplugin.json \
    the_picture_of_dorian_gray.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
