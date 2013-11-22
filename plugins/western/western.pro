CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    tests \
    src

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
