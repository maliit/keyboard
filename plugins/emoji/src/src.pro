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
    emojiplugin.h

TARGET          = $$qtLibraryTarget(emojiplugin)

EXAMPLE_FILES = emojiplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${UBUNTU_KEYBOARD_LIB_DIR}/emoji/

lang_db_emoji.commands += \
  rm -f $$PWD/database_emoji.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_emoji.db $$PWD/free_ebook.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_emoji.db $$PWD/free_ebook.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_emoji.db $$PWD/free_ebook.txt
lang_db_emoji.files += $$PWD/database_emoji.db
lang_db_emoji_install.path = $$PLUGIN_INSTALL_PATH
lang_db_emoji_install.files += $$PWD/database_emoji.db

QMAKE_EXTRA_TARGETS += lang_db_emoji lang_db_emoji_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_emoji_install

OTHER_FILES += \
    emojiplugin.json \
    free_ebook.txt

LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage -lhunspell

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
