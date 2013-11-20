CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    pinyin \
    western

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
