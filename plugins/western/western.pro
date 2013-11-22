CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    src \
    tests

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
