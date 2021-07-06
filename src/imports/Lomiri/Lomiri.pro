CONFIG += ordered 
TEMPLATE = subdirs
SUBDIRS = Keyboard

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
