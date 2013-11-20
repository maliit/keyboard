CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    example \
    western

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
