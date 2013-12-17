TOP_BUILDDIR = $$OUT_PWD/../../..
TOP_SRCDIR = $$PWD/../../..

include($${TOP_SRCDIR}/config.pri)

PINYIN_DATA_DIR = "$$system(pkg-config --variable pkgdatadir libpinyin)/data"
DEFINES += PINYIN_DATA_DIR=\\\"$${PINYIN_DATA_DIR}\\\"

TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += \
    $${TOP_SRCDIR}/src/ \
    $${TOP_SRCDIR}/src/lib/logic

HEADERS         = \
                  pinyinadapter.h \
                  pinyinplugin.h \
                  chineselanguagefeatures.h
SOURCES         = \
                  pinyinadapter.cpp \
                  pinyinplugin.cpp \
                  chineselanguagefeatures.cpp

TARGET          = $$qtLibraryTarget(pinyinplugin)

EXAMPLE_FILES = pinyinplugin.json

# install
target.path = $${UBUNTU_KEYBOARD_LIB_DIR}/zh/
INSTALLS += target

OTHER_FILES += \
    pinyinplugin.json

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
PKGCONFIG += libpinyin
