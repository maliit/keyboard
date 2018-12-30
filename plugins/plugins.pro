CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    westernsupport \
    ar \
    az \
    bg \
    bs \
    ca \
    cs \
    da \
    de \
    emoji \
    el \
    en \
    eo \
    es \
    fa \
    fi \
    fr \
    fr_ch \
    gd \
    he \
    hr \
    hu \
    is \
    it \
    ja \
    lt \
    lv \
    ko \
    nb \
    nl \
    pl \
    pt \
    ro \
    ru \
    tr \
    sl \
    sr \
    sv \
    uk \
    pinyin \
    chewing \

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
