include(../config.pri)

VERSION = 0.2.0
TARGET = $${MALIIT_KEYBOARD_TARGET}
TEMPLATE = lib
QT += core gui quick
CONFIG += staticlib

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
PKGCONFIG += libpinyin
LIBS += libpinyin

include(models/models.pri)
include(logic/logic.pri)
include(parser/parser.pri)

HEADERS += coreutils.h
SOURCES += coreutils.cpp

include(../word-prediction.pri)
