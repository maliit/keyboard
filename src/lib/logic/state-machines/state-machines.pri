STATE_MACHINES_DIR = $$LOGIC_DIR/state-machines

HEADERS += \
    logic/state-machines/abstractstatemachine.h \
    logic/state-machines/shiftmachine.h \
    logic/state-machines/viewmachine.h \
    logic/state-machines/deadkeymachine.h \

SOURCES += \
    logic/state-machines/abstractstatemachine.cpp \
    logic/state-machines/shiftmachine.cpp \
    logic/state-machines/viewmachine.cpp \
    logic/state-machines/deadkeymachine.cpp \

DEPENDPATH += $$STATE_MACHINES_DIR
