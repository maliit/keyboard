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
    fi \
    fr \
    gd \
    he \
    hr \
    hu \
    is \
    it \
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

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
