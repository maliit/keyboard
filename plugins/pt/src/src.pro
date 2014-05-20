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
    portugueseplugin.h

TARGET          = $$qtLibraryTarget(portugueseplugin)

EXAMPLE_FILES = portugueseplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $$UBUNTU_KEYBOARD_LIB_DIR/pt/

lang_db_pt.commands += \
  rm -f $$PWD/database_pt.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_pt.db $$PWD/historias_sem_data.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_pt.db $$PWD/historias_sem_data.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_pt.db $$PWD/historias_sem_data.txt
lang_db_pt.files += $$PWD/database_pt.db

lang_db_pt_install.files += $$PWD/database_pt.db
lang_db_pt_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_pt lang_db_pt_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_pt_install

OTHER_FILES += \
    portugueseplugin.json \
    historias_sem_data.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
