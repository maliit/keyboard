#!/bin/bash
if [ "$SNAP_ARCH" == "amd64" ]; then
    ARCH="x86_64-linux-gnu"
elif [ "$SNAP_ARCH" == "armhf" ]; then
    ARCH="arm-linux-gnueabihf"
else
    ARCH="$SNAP_ARCH-linux-gnu"
fi

# Qt Platform to Mir
export QTCHOOSER_NO_GLOBAL_DIR=1
export QT_SELECT=snappy-qt5

# Qt Libs
export LD_LIBRARY_PATH=$SNAP/usr/lib/$ARCH/qt5/libs:$LD_LIBRARY_PATH

# Qt Modules
export QT_PLUGIN_PATH=$SNAP/usr/lib/$ARCH/qt5/plugins
export QML2_IMPORT_PATH=$QML2_IMPORT_PATH:$SNAP/usr/lib/$ARCH/qt5/qml
export QML2_IMPORT_PATH=$QML2_IMPORT_PATH:$SNAP/lib/$ARCH

# Necessary for the SDK to find the translations directory
export APP_DIR=$SNAP

export PRESAGE_PREFIX_PATH=$SNAP
export KEYBOARD_PREFIX_PATH=$SNAP
export MALIIT_PLUGIN_PATH=$SNAP/usr/lib/maliit/plugins/ 

echo $MALIIT_PLUGINS_DATADIR

export PATH=$SNAP/usr/bin/:$PATH
exec maliit-server
