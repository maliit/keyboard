TEMPLATE = subdirs
SUBDIRS = \
    common \
    dynamic-layout \
    editor \
    language-layout-switching \
#    preedit-string \
    repeat-backspace \
    word-candidates \
    language-layout-loading \
    qml-api-tests \
    qml-test-app \
    unittests \
    word-candidates \
    wordengine \

CONFIG += ordered
QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
