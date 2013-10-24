TEMPLATE = subdirs
SUBDIRS = \
    api-test \
    qml-api-tests \
    qml-test-app \

CONFIG += ordered
QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
