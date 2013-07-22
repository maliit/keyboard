TEMPLATE = subdirs
SUBDIRS = \
    common \
    editor \
    language-layout-switching \
    preedit-string \
    repeat-backspace \
    word-candidates \
    language-layout-loading \
    qml-test-app \
    unittests \

CONFIG += ordered
QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
