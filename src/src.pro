CONFIG += ordered 
TEMPLATE = subdirs
SUBDIRS = \
    lib \
    view \
    plugin \
#    benchmark \


#!notests {
#    SUBDIRS += tests
#}

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
