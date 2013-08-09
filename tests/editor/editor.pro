include(../../src/config.pri)
include(../common-check.pri)
# include(../../src/config-plugin.pri)

TOP_BUILDDIR = $${OUT_PWD}/../..
TEMPLATE = app
TARGET = ut_editor
target.path = $$INSTALL_BIN


QMAKE_LFLAGS_RPATH=$${TOP_BUILDDIR}/src/plugin
INCLUDEPATH += ../ ../../src/lib ../../src

#LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB}
LIBS += -L$${TOP_BUILDDIR}/src/plugin -lubuntu-keyboard-plugin

##LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} -l$${MALIIT_KEYBOARD_PLUGIN_TARGET}
#-Lubuntu-keyboard-plugin
##PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} -l$${MALIIT_KEYBOARD_PLUGIN_TARGET}

##LIBS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}
# LIBS += -L$${MALIIT_KEYBOARD_PLUGIN_LIB} -L$${MALIIT_KEYBOARD_VIEW_LIB} -L$${MALIIT_KEYBOARD_LIB}
##PRE_TARGETDEPS += $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_PLUGIN_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_VIEW_LIB} $${TOP_BUILDDIR}/$${MALIIT_KEYBOARD_LIB}

HEADERS += \
    wordengineprobe.h \

SOURCES += \
    wordengineprobe.cpp \
    main.cpp \

QT = core testlib gui
INSTALLS += target
