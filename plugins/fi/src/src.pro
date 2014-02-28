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
    finnishplugin.h

TARGET          = $$qtLibraryTarget(finnishplugin)

EXAMPLE_FILES = finnishplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/fi/

lang_db_fi.path = $$PLUGIN_INSTALL_PATH
lang_db_fi.commands += \
  rm -f $$PWD/database_fi.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_fi.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_fi.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_fi.db $$PWD/free_ebook.txt

lang_db_fi.files += $$PWD/database_fi.db
QMAKE_EXTRA_TARGETS += lang_db_fi

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_fi

OTHER_FILES += \
    finnishplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
