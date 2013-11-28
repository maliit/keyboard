LOGIC_DIR = ./logic

HEADERS += \
    logic/layouthelper.h \
    logic/layoutupdater.h \
    logic/keyboardloader.h \
    logic/style.h \
    logic/abstractwordengine.h \
    logic/wordengine.h \
    logic/abstractlanguagefeatures.h \
    logic/eventhandler.h \
    logic/languageplugininterface.h \
 #   logic/westernlanguagesplugin.h \
#    logic/westernlanguagefeatures.h \
#    logic/spellchecker.h \
#    logic/candidatescallback.h

SOURCES += \
    logic/layouthelper.cpp \
    logic/layoutupdater.cpp \
    logic/keyboardloader.cpp \
    logic/style.cpp \
    logic/abstractwordengine.cpp \
    logic/wordengine.cpp \
    logic/eventhandler.cpp \
#    logic/westernlanguagesplugin.cpp \
#    logic/westernlanguagefeatures.cpp \
#    logic/spellchecker.cpp \
#    logic/candidatescallback.cpp

DEPENDPATH += $$LOGIC_DIR

#LIBS += -lpresage
#DEFINES += HUNSPELL_DICT_PATH=\\\"$$HUNSPELL_DICT_PATH\\\"
