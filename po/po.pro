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

PO_FILES = $$system(ls *.po)

install_mo_commands =
for(po_file, PO_FILES) {
  mo_file = $$replace(po_file,.po,.mo)
  !isEmpty(install_mo_commands): install_mo_commands += &&
  install_mo_commands += msgfmt $$po_file -o $$mo_file
  mo_name = $$replace(po_file,.po,)
  mo_targetpath = $(INSTALL_ROOT)/usr/share/locale/$${mo_name}/LC_MESSAGES
  mo_target = $${mo_targetpath}/$${PROJECTNAME}.mo
  install_mo_commands += && test -d $$mo_targetpath || mkdir -p $$mo_targetpath
  install_mo_commands += && cp $$mo_file $$mo_target
}

install.commands = $$install_mo_commands
QMAKE_EXTRA_TARGETS += install
