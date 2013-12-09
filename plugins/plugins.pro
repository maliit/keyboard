CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    westernsupport \
    de \
    en \
    es \
    fr \
    it \
    pt \
    pinyin \

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
