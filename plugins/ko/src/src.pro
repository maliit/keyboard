TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += \
    $${TOP_SRCDIR}/src/ \
    $${TOP_SRCDIR}/src/lib/ \
    $${TOP_SRCDIR}/src/lib/logic/ \
    $${TOP_SRCDIR}/plugins/westernsupport/

HEADERS         = \
    koreanplugin.h \
    koreanlanguagefeatures.h \
    $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.h \
    $${TOP_SRCDIR}/plugins/westernsupport/spellchecker.h \
    $${TOP_SRCDIR}/plugins/westernsupport/spellpredictworker.h \
    $${TOP_SRCDIR}/plugins/westernsupport/candidatescallback.h \

SOURCES         = \
    koreanplugin.cpp \
    koreanlanguagefeatures.cpp \
    $${TOP_SRCDIR}/src/lib/logic/abstractlanguageplugin.cpp \
    $${TOP_SRCDIR}/plugins/westernsupport/spellchecker.cpp \
    $${TOP_SRCDIR}/plugins/westernsupport/spellpredictworker.cpp \
    $${TOP_SRCDIR}/plugins/westernsupport/candidatescallback.cpp \


TARGET          = $$qtLibraryTarget(koplugin)

EXAMPLE_FILES = koreanplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/ko/

lang_db_ko.commands += \
  rm -f $$PWD/database_ko.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_ko.db $$PWD/korean.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_ko.db $$PWD/korean.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_ko.db $$PWD/korean.txt
lang_db_ko.files += $$PWD/database_ko.db

lang_db_ko_install.files += $$PWD/database_ko.db
lang_db_ko_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_ko lang_db_ko_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_ko_install

OTHER_FILES += \
    koreanplugin.json \
    korean.txt

# hunspell
CONFIG += link_pkgconfig
PKGCONFIG += hunspell
DEFINES += HAVE_HUNSPELL

# presage
LIBS += -lpresage
DEFINES += HUNSPELL_DICT_PATH=\\\"$$HUNSPELL_DICT_PATH\\\"

