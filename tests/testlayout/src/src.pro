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
    testlayoutplugin.h

TARGET          = $$qtLibraryTarget(testlayoutplugin)

EXAMPLE_FILES = testlayoutplugin.json

# generate database for presage:
PLUGIN_INSTALL_PATH = $${LOMIRI_KEYBOARD_TEST_DIR}/testlayout/

lang_db_testlayout.commands += \
  rm -f $$PWD/database_testlayout.db && \
  text2ngram -n 1 -l -f sqlite -o $$PWD/database_testlayout.db $$PWD/empty.txt && \
  text2ngram -n 2 -l -f sqlite -o $$PWD/database_testlayout.db $$PWD/empty.txt && \
  text2ngram -n 3 -l -f sqlite -o $$PWD/database_testlayout.db $$PWD/empty.txt
lang_db_testlayout.files += $$PWD/database_testlayout.db

lang_db_testlayout_install.files += $$PWD/database_testlayout.db
lang_db_testlayout_install.path = $$PLUGIN_INSTALL_PATH

QMAKE_EXTRA_TARGETS += lang_db_testlayout lang_db_testlayout_install

target.path = $$PLUGIN_INSTALL_PATH
INSTALLS += target lang_db_testlayout_install

OTHER_FILES += \
    testlayoutplugin.json \
    the_picture_of_dorian_gray.txt

CONFIG += link_pkgconfig
PKGCONFIG += hunspell
LIBS += $${TOP_BUILDDIR}/plugins/plugins/libwesternsupport.a -lpresage

INCLUDEPATH += $${TOP_SRCDIR}/plugins/westernsupport
DEPENDPATH += $${TOP_SRCDIR}/plugins/westernsupport
