TEMPLATE = subdirs
SUBDIRS = \
    common \
    dynamic-layout \
    editor \
    keyboardsettings \
    language-layout-loading \
    language-layout-switching \
#    preedit-string \
    qml-api-tests \
    qml-test-app \
    repeat-backspace \
    unittests \
    word-candidates \
    wordengine \

CONFIG += ordered
QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
