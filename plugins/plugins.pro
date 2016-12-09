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
    el \
    en \
    es \
    fa \
    fi \
    fr \
    gd \
    he \
    hr \
    hu \
    is \
    it \
    ja \
    lv \
    ko \
    nb \
    nl \
    pl \
    pt \
    ro \
    ru \
    sl \
    sr \
    sv \
    uk \
    pinyin \
    chewing \

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
