# Linker optimization for release build
QMAKE_LFLAGS_RELEASE+=-Wl,--as-needed
# Compiler warnings are error if the build type is debug.
# Except when we pass a CONFIG+=no-werror as a workaround for QTBUG-18092
no-werror {
    QMAKE_CXXFLAGS_DEBUG+=-O0
} else {
    QMAKE_CXXFLAGS_DEBUG+=-Werror -O0
}

#CONFIG += no_keywords

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
    MALIIT_DEFAULT_PROFILE = ubuntu
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

LOMIRI_KEYBOARD_PACKAGENAME = ubuntu-keyboard
LOMIRI_KEYBOARD_VERSION = $$system(cat $$PWD/VERSION)
LOMIRI_KEYBOARD_DATA_DIR = "$${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu"
LOMIRI_KEYBOARD_LIB_DIR = "$${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu/lib"
LOMIRI_KEYBOARD_TEST_DIR = "/usr/share/maliit/tests/ubuntu-keyboard"
LOMIRI_KEYBOARD_HEADERS_DIR = "$${MALIIT_PLUGINS_DATA_DIR}/com/ubuntu/include"

PINYIN_DATA_DIR = "$$system(pkg-config --variable pkgdatadir libpinyin)/data"

DEFINES += LOMIRI_KEYBOARD_DATA_DIR=\\\"$${LOMIRI_KEYBOARD_DATA_DIR}\\\"
DEFINES += MALIIT_PLUGINS_DATA_DIR=\\\"$${MALIIT_PLUGINS_DATA_DIR}\\\"
DEFINES += PINYIN_DATA_DIR=\\\"$${PINYIN_DATA_DIR}\\\"

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

LOMIRI_KEYBOARD_TARGET = ubuntu-keyboard
LOMIRI_KEYBOARD_VIEW_TARGET = ubuntu-keyboard-view
LOMIRI_KEYBOARD_PLUGIN_TARGET = ubuntu-keyboard-plugin

LOMIRI_KEYBOARD_LIB = src/lib/$$maliitStaticLib($${LOMIRI_KEYBOARD_TARGET})
LOMIRI_KEYBOARD_VIEW_LIB = src/view/$$maliitStaticLib($${LOMIRI_KEYBOARD_VIEW_TARGET})
LOMIRI_KEYBOARD_PLUGIN_LIB = src/plugin/$$maliitDynamicLib($${LOMIRI_KEYBOARD_PLUGIN_TARGET})
