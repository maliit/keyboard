import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.3

import MaliitKeyboard 2.0

import "keys/"

Rectangle{
    id: actionsToolbar
	
    color: Theme.backgroundColor
    
    anchors {
        left: parent.left
        right: parent.right
    }

    states: [
        State {
            name: "wordribbon"
        
            AnchorChanges {
                target: actionsToolbar
                anchors.top: undefined
                anchors.bottom: keyboardComp.top
            }
        },
        State {
            name: "top"

            AnchorChanges {
                target: actionsToolbar
                anchors.top: parent.top
                anchors.bottom: undefined
            }
         }
    ]
    
    // Disable clicking behind the toolbar
    MouseArea {
        anchors.fill: parent
        z: -1
    }
    
    RowLayout {
        anchors.fill: parent
        
        RowLayout {
            id: leadingActionBar
            
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            Layout.fillHeight: true

            readonly property list<Action> actions: [
                Action { text: qsTr("Select All"); icon.name: "edit-select-all"; onTriggered: fullScreenItem.selectAll(); },
                Action { text: qsTr("Redo"); icon.name: "redo"; onTriggered: fullScreenItem.redo();},
                Action { text: qsTr("Undo"); icon.name: "undo"; onTriggered: fullScreenItem.undo();}
            ]

            Repeater {
                delegate: ActionsToolbarButton {
                    Layout.fillHeight: true
                    fullLayout: fullScreenItem.width > Device.gu(80)
                }
                model: leadingActionBar.actions
            }
        }

        RowLayout {
            id: trailingActionBar
            
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

            readonly property list<Action> actions: [
                // TODO: Disabled dynamic visibility of copy and cut buttons until input_method.hasSelection is working properly in QtWebEngine
                // ubports/ubuntu-touch#1157 <https://github.com/ubports/ubuntu-touch/issues/1157>
                Action { text: qsTr("Paste"); icon.name: "edit-paste"; onTriggered: fullScreenItem.paste(); },
                Action { text: qsTr("Copy"); icon.name: "edit-copy"; enabled: input_method.hasSelection; onTriggered: {fullScreenItem.copy(); fullScreenItem.sendLeftKey();} },
                Action { text: qsTr("Cut"); icon.name: "edit-cut"; enabled: input_method.hasSelection; onTriggered: fullScreenItem.cut(); }
            ]
            Repeater {
                delegate: ActionsToolbarButton {
                    Layout.fillHeight: true
                    fullLayout: fullScreenItem.width > Device.gu(45)
                }
                model: trailingActionBar.actions
            }
        }
    }
}
