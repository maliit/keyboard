LOGIC_DIR = ./logic

HEADERS += \
#    logic/layouthelper.h \
#    logic/layoutupdater.h \
#    logic/keyboardloader.h \
    logic/style.h \
    logic/abstractwordengine.h \
    logic/wordengine.h \
    logic/abstractlanguagefeatures.h \
    logic/eventhandler.h \
    logic/languageplugininterface.h \

SOURCES += \
#    logic/layouthelper.cpp \
#    logic/layoutupdater.cpp \
#    logic/keyboardloader.cpp \
    logic/style.cpp \
    logic/abstractwordengine.cpp \
    logic/wordengine.cpp \
    logic/eventhandler.cpp \

DEPENDPATH += $$LOGIC_DIR
