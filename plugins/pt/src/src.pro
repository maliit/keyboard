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
QMAKE_POST_LINK = text2ngram -n 1 -l -f sqlite -o $$TOP_BUILDDIR/database_pt.db $$PWD/historias_sem_data.txt
QMAKE_CLEAN     += $$TOP_BUILDDIR/database_pt.db

# install
target.path = $${UBUNTU_KEYBOARD_LIB_DIR}
target.files += $$PWD/database_pt.db
INSTALLS += target

OTHER_FILES += \
    portugueseplugin.json

LIBS += -L$${TOP_SRCDIR}/plugins/plugins/ -lwesternsupport

INCLUDEPATH += $$PWD/../../westernsupport
DEPENDPATH += $$PWD/../../westernsupport
