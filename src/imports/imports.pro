CONFIG += ordered 
TEMPLATE = subdirs
SUBDIRS = Ubuntu



QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
