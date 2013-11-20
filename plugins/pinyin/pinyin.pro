PINYIN_DATA_DIR = "$$system(pkg-config --variable pkgdatadir libpinyin)/data"
DEFINES += PINYIN_DATA_DIR=\\\"$${PINYIN_DATA_DIR}\\\"

TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../../src/
HEADERS         =
                  pinyinadapter.h \
                  pinyinplugin.h
SOURCES         = \
                  pinyinadapter.cpp \
                  pinyinplugin.cpp
TARGET          = $$qtLibraryTarget(pinyinplugin)
DESTDIR         = ../plugins

EXAMPLE_FILES = pinyinplugin.json

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/pinyinplugin/plugin
INSTALLS += target

OTHER_FILES += \
    pinyinplugin.json

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
PKGCONFIG += libpinyin
