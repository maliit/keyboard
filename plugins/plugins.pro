CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    westernsupport \
    ar \
    bs \
    cs \
    da \
    de \
    en \
    es \
    fi \
    fr \
    he \
    hu \
    it \
    nl \
    pl \
    pt \
    ru \
    sr \
    sv \
    pinyin \

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
