TEMPLATE = subdirs
SUBDIRS = \
    qmltests \
    unittests \

CONFIG += ordered
QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
