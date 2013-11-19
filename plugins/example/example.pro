PINYIN_DATA_DIR = "$$system(pkg-config --variable pkgdatadir libpinyin)/data"
DEFINES += PINYIN_DATA_DIR=\\\"$${PINYIN_DATA_DIR}\\\"

TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../../src/
HEADERS         = exampleplugin.h \
                  pinyinadapter.h
    exampleplugin.h
SOURCES         = exampleplugin.cpp \
                  pinyinadapter.cpp
TARGET          = $$qtLibraryTarget(exampleplugin)
DESTDIR         = ../plugins

EXAMPLE_FILES = exampleplugin.json

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tools/exampleplugin/plugin
INSTALLS += target

OTHER_FILES += \
    exampleplugin.json

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
PKGCONFIG += libpinyin
DEFINES += HAVE_PINYIN
