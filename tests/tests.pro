TEMPLATE = subdirs
SUBDIRS = \
    qmltests \
    testlayout \
    unittests \

CONFIG += ordered
QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
