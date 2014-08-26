TEMPLATE = subdirs

PROJECTNAME = ubuntu-keyboard

SOURCECODE = ../qml/*.qml \
             ../qml/keys/*.qml
             ../qml/languages/*.qml
             ../plugins/*/qml/*.qml

message("")
message(" Project Name: $$PROJECTNAME ")
message(" Source Code: $$SOURCECODE ")
message("")
message(" Run 'make pot' to generate the pot file from source code. ")
message("")

## Generate pot file 'make pot'
potfile.target = pot
potfile.commands = xgettext \
                   -o $${PROJECTNAME}.pot \
                   --copyright=\"Canonical Ltd. \" \
                   --package-name $${PROJECTNAME} \
                   --qt --c++ --add-comments=TRANSLATORS \
                   --keyword=tr --keyword=tr:1,2 \
                   $${SOURCECODE} $${DESKTOPFILETEMP}
QMAKE_EXTRA_TARGETS += potfile

