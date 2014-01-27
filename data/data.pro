include(../config.pri)

TARGET = dummy
TEMPLATE = lib

schemas.path = $${PREFIX}/share/glib-2.0/schemas
schemas.files = schemas/*.gschema.xml

styles.path = $${UBUNTU_KEYBOARD_DATA_DIR}
styles.files = styles

INSTALLS += schemas styles
