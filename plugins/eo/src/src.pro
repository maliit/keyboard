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
    esperantoplugin.h

TARGET          = $$qtLibraryTarget(eoplugin)

EXAMPLE_FILES = esperantoplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_PLUGIN_DIR}/eo/

lang_db_eo.commands += \
  rm -f $$PWD/database_eo.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_eo.db $$PWD/alicio_en_mirlando.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_eo.db $$PWD/alicio_en_mirlando.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_eo.db $$PWD/alicio_en_mirlando.txt
lang_db_eo.files += $$PWD/database_eo.db

lang_db_eo_install.files += $$PWD/database_eo.db
lang_db_eo_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_eo lang_db_eo_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_eo_install

OTHER_FILES += \
    esperantoplugin.json \
    alicio_en_mirlando.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
