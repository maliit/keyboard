# Linker optimization for release build
QMAKE_LFLAGS_RELEASE+=-Wl,--as-needed
# Compiler warnings are error if the build type is debug.
# Except when we pass a CONFIG+=no-werror as a workaround for QTBUG-18092
no-werror {
    QMAKE_CXXFLAGS_DEBUG+=-O0
} else {
    QMAKE_CXXFLAGS_DEBUG+=-Werror -O0
}

CONFIG += no_keywords

# The feature maliit-defines initializes some variables related for MALIIT, such as installation paths
# here have to load it early, to start using the defines immediately
!load(maliit-defines) {
   error(Cannot find $$[QT_INSTALL_DATA]/mkspecs/features/maliit-defines.prf. Probably Maliit framework not installed)
}
# This enables the maliit library for C++ code
CONFIG += maliit-plugins

isEmpty(PREFIX) {
   PREFIX = $$MALIIT_PREFIX
}

isEmpty(LIBDIR) {
   LIBDIR = $$PREFIX/lib
}

isEmpty(MALIIT_DEFAULT_PROFILE) {
    MALIIT_DEFAULT_PROFILE = nokia-n9
}

isEmpty(HUNSPELL_DICT_PATH) {
    HUNSPELL_DICT_PATH = $$PREFIX/share/hunspell
}

contains(QT_CONFIG, embedded) {
    CONFIG += qws
}

INSTALL_BIN = $$PREFIX/bin
INSTALL_LIBS = $$LIBDIR
INSTALL_HEADERS = $$PREFIX/include
INSTALL_DOCS = $$PREFIX/share/doc

enable-opengl {
    QT += opengl
    DEFINES += MALIIT_KEYBOARD_HAVE_OPENGL
}

MALIIT_PACKAGENAME = maliit-plugins
MALIIT_VERSION = $$system(cat $$PWD/VERSION)




MALIIT_KEYBOARD_DATA_DIR = "$${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu"

DEFINES += MALIIT_PLUGINS_DATA_DIR=\\\"$${MALIIT_PLUGINS_DATA_DIR}\\\"
DEFINES += MALIIT_KEYBOARD_DATA_DIR=\\\"$${MALIIT_KEYBOARD_DATA_DIR}\\\"

unix {
    MALIIT_STATIC_PREFIX=lib
    MALIIT_STATIC_SUFFIX=.a
    MALIIT_DYNAMIC_PREFIX=lib
    MALIIT_DYNAMIC_SUFFIX=.so
}

win32 {
    # qmake puts libraries in subfolders on Windows
    release {
        MALIIT_STATIC_PREFIX=release/lib
        MALIIT_DYNAMIC_PREFIX=release/
    }
    debug {
        MALIIT_STATIC_PREFIX=debug/lib
        MALIIT_DYNAMIC_PREFIX=debug/
    }

    MALIIT_STATIC_SUFFIX=.a
    MALIIT_DYNAMIC_SUFFIX=.dll
}

defineReplace(maliitStaticLib) {
    return($${MALIIT_STATIC_PREFIX}$${1}$${MALIIT_STATIC_SUFFIX})
}

defineReplace(maliitDynamicLib) {
    return($${MALIIT_DYNAMIC_PREFIX}$${1}$${MALIIT_DYNAMIC_SUFFIX})
}

MALIIT_KEYBOARD_TARGET = ubuntu-keyboard
MALIIT_KEYBOARD_VIEW_TARGET = ubuntu-keyboard-view
MALIIT_KEYBOARD_PLUGIN_TARGET = ubuntu-keyboard-plugin

MALIIT_KEYBOARD_LIB = src/lib/$$maliitStaticLib($${MALIIT_KEYBOARD_TARGET})
MALIIT_KEYBOARD_VIEW_LIB = src/view/$$maliitStaticLib($${MALIIT_KEYBOARD_VIEW_TARGET})
MALIIT_KEYBOARD_PLUGIN_LIB = src/plugin/$$maliitDynamicLib($${MALIIT_KEYBOARD_PLUGIN_TARGET})


UBUNTU_KEYBOARD_DATA_DIR = "$${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu"
