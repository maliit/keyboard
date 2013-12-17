TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
    common \
    ut_editor \
    ut_keyboardgeometry \
    ut_keyboardsettings \
    ut_languagefeatures \
#    ut_preedit-string \
    ut_repeat-backspace \
    ut_text \
    ut_word-candidates \
##    ut_wordengine \

ut_editor.depends = common

QMAKE_EXTRA_TARGETS += check
check.target = check
check.CONFIG = recursive
