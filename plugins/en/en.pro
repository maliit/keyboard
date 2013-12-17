CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS = \
    src \
    qml

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
