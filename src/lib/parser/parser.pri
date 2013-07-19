PARSER_DIR = ./parser

HEADERS += \
    parser/alltagtypes.h \
    parser/layoutparser.h \
    parser/tagbindingcontainer.h \
    parser/tagbinding.h \
    parser/tagextended.h \
    parser/tagkeyboard.h \
    parser/tagkey.h \
    parser/taglayout.h \
    parser/tagmodifiers.h \
    parser/tagrowcontainer.h \
    parser/tagrowelement.h \
    parser/tagrow.h \
    parser/tagsection.h \
    parser/tagspacer.h

SOURCES += \
    parser/layoutparser.cpp \
    parser/tagbindingcontainer.cpp \
    parser/tagbinding.cpp \
    parser/tagextended.cpp \
    parser/tagkeyboard.cpp \
    parser/tagkey.cpp \
    parser/taglayout.cpp \
    parser/tagmodifiers.cpp \
    parser/tagrowcontainer.cpp \
    parser/tagrowelement.cpp \
    parser/tagrow.cpp \
    parser/tagsection.cpp \
    parser/tagspacer.cpp

DEPENDPATH += $$PARSER_DIR
