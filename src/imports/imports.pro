CONFIG += ordered 
TEMPLATE = subdirs
SUBDIRS = Lomiri



QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
