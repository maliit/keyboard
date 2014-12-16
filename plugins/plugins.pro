CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    westernsupport \
    ar \
    az \
    bs \
    ca \
    cs \
    da \
    de \
    emoji \
    en \
    es \
    fi \
    fr \
    he \
    hr \
    hu \
    it \
    nl \
    pl \
    pt \
    ru \
    sr \
    sv \
    uk \
    pinyin \

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
