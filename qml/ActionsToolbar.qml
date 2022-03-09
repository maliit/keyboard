import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.3

import MaliitKeyboard 2.0

import "keys/"

Item {
    id: actionsToolbar
	
    anchors {
        left: parent.left
        right: parent.right
    }

    // Disable clicking behind the toolbar
    MouseArea {
        anchors.fill: parent
        z: -1
    }
    
    RowLayout {
        anchors.fill: parent
        
        Row {
            id: leadingActionBar
            
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.fillHeight: true

            readonly property list<Action> actions: [
                Action { text: qsTr("Undo"); icon.name: "edit-undo-symbolic"; onTriggered: fullScreenItem.undo();},
                Action { text: qsTr("Redo"); icon.name: "edit-redo-symbolic"; onTriggered: fullScreenItem.redo();},
                Action { text: qsTr("Select All"); icon.name: "edit-select-all-symbolic"; onTriggered: fullScreenItem.selectAll(); }
            ]

            Repeater {
                delegate: ActionsToolbarButton {
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    fullLayout: fullScreenItem.width > Device.gu(80)
                }
                model: leadingActionBar.actions
            }
        }

        Row {
            id: trailingActionBar
            
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

            readonly property list<Action> actions: [
                // TODO: Disabled dynamic visibility of copy and cut buttons until input_method.hasSelection is working properly in QtWebEngine
                // ubports/ubuntu-touch#1157 <https://github.com/ubports/ubuntu-touch/issues/1157>
                Action { text: qsTr("Cut"); icon.name: "edit-cut-symbolic"; onTriggered: {console.log("cut"); fullScreenItem.cut();} },
                Action { text: qsTr("Copy"); icon.name: "edit-copy-symbolic"; onTriggered: {fullScreenItem.copy(); fullScreenItem.sendLeftKey();} },
                Action { text: qsTr("Paste"); icon.name: "edit-paste-symbolic"; onTriggered: fullScreenItem.paste(); }
            ]
            Repeater {
                delegate: ActionsToolbarButton {
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                    }
                    fullLayout: fullScreenItem.width > Device.gu(45)
                }
                model: trailingActionBar.actions
            }
        }
    }
}
