CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    example

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
