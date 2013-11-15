include(config.pri)

!isEmpty(HELP) {
    # Output help
    help_string = \
        Important build options: \
        \\n\\t PREFIX: Install prefix for tools. Default: Maliit framework prefix \
        \\n\\t\\t Note that the plugins themselves are installed under Maliit framework prefix \
        \\n\\t LIBDIR: Library install directory. Default: $$PREFIX/lib \
        \\n\\t MALIIT_DEFAULT_PROFILE: Default keyboard style. Default: nokia-n9 \
        \\n\\t HUNSPELL_DICT_PATH: Path to hunspell dictionaries. Default: $$PREFIX/share/hunspell \
        \\nRecognised CONFIG flags: \
        \\n\\t enable-presage: Use presage to calculate word candidates \
        \\n\\t enable-hunspell: Use hunspell for error correction \
        \\n\\t enable-pinyin: Use libpinyin as chinese input method \
        \\n\\t notests: Do not attempt to build tests \
        \\n\\t nodoc: Do not build documentation \
        \\nInfluential environment variables: \
        \\n\\t QMAKEFEATURES A mkspecs/features directory list to look for features. \
        \\n\\t\\t Use it if a dependency is installed to non-default location. \
        \\n\\t\\t Note that it must also be passed to qmake calls in subdirectories. \
        \\nExamples: \
        \\n\\t qmake \
        \\n\\t qmake PREFIX=/usr LIBDIR=/usr/lib64 \

    !build_pass:system(echo -e \"$$help_string\")
} else {
    config_string = Tip: Run qmake HELP=1 for a list of all supported build options

    !build_pass:system(echo -e \"$$config_string\")
}

CONFIG += ordered 
TEMPLATE = subdirs

DIST_NAME = $$UBUNTU_KEYBOARD_PACKAGENAME-$$UBUNTU_KEYBOARD_VERSION
DIST_PATH = $$OUT_PWD/$$DIST_NAME
TARBALL_SUFFIX = .tar.bz2
TARBALL_PATH = $$DIST_PATH$$TARBALL_SUFFIX

QMAKE_EXTRA_TARGETS += dist
dist.target = dist
dist.commands += git archive HEAD --prefix=$$DIST_NAME/ | bzip2 > $$TARBALL_PATH;
dist.commands += md5sum $$TARBALL_PATH | cut -d \' \' -f 1 > $$DIST_PATH\\.md5

# The 'make coverage' target
# Builds plugins with coverage libs in separate directory.
COVERAGE_CONFIG_STRING = CONFIG+=debug CONFIG+=nodoc CONFIG+=no-werror

enable-presage {
    COVERAGE_CONFIG_STRING += CONFIG+=enable-presage
    message(presage is enabled)
}

enable-hunspell {
    COVERAGE_CONFIG_STRING += CONFIG+=enable-hunspell
    message(hunspell is enabled)
}

disable-preedit {
    COVERAGE_CONFIG_STRING += CONFIG+=disable-preedit
    message(preedit is enabled)
}

enable-pinyin {
    COVERAGE_CONFIG_STRING += CONFIG+=enable-pinyin
    CONFIG += link_pkgconfig
    PKGCONFIG += glib-2.0
    PKGCONFIG += libpinyin
    DEFINES += HAVE_PINYIN
    message(pinyin is enabled)
}


QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive

COVERAGE_DIR = coverage-build

QMAKE_EXTRA_TARGETS += coverage
coverage.target = coverage

coverage.commands += $(MKDIR) $$COVERAGE_DIR &&
coverage.commands += cd $$COVERAGE_DIR &&
coverage.commands += $(QMAKE) -r PREFIX=\"$$PREFIX\" LIBDIR=\"$$LIBDIR\" MALIIT_DEFAULT_PROFILE=\"$$MALIIT_DEFAULT_PROFILE\" HUNSPELL_DICT_PATH=\"$$HUNSPELL_DICT_PATH\" $$COVERAGE_CONFIG_STRING LIBS+=\"$$LIBS\" INCLUDEPATH+=\"$$INCLUDEPATH\" LIBS+=-lgcov QMAKE_CXXFLAGS_DEBUG+=\"-fprofile-arcs -ftest-coverage\" QMAKE_LFLAGS_DEBUG+=\"-fprofile-arcs -ftest-coverage\" $$PWD/maliit-plugin-ubuntu.pro.coverage &&
coverage.commands += make $(MAKEFLAGS) coverage

coverage.clean_commands = rm -rf $$COVERAGE_DIR


SUBDIRS = \
    src \
    data \
    qml \
    benchmark \

!notests {
    SUBDIRS += tests
}


tests.target = test
tests.command = cd tests/editor
QMAKE_EXTRA_TARGETS += tests

