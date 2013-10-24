
# to be run on the device

#LD_LIBRARY_PATH=src/plugin:$LD_LIBRARY_PATH tests/editor/ut_editor
#LD_LIBRARY_PATH=src/plugin:$LD_LIBRARY_PATH tests/wordengine/ut_wordengine
#LD_LIBRARY_PATH=src/plugin:$LD_LIBRARY_PATH tests/dynamic-layout/ut_dynamiclayout
#LD_LIBRARY_PATH=src/plugin:$LD_LIBRARY_PATH tests/language-layout-switching/language-layout-switching
make check

qmltestrunner -import maliit-keyboard/tests/qmltests/qml-api-tests/imports/ -input maliit-keyboard/tests/qmltests/qml-api-tests/tst_inputMethodHints.qml
