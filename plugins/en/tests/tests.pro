TEMPLATE = subdirs
SUBDIRS = \
    ut_languagefeatures

CONFIG += ordered
QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
