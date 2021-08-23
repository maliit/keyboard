import QtQuick 2.9
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import MaliitKeyboard 2.0

import "keys/"

RowLayout {
    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
        margins: Device.gu(1)
        topMargin: toolbar.height + Device.gu(1)
    }
    
    FloatingActionKey {
        id: startLineButton
        
        Layout.alignment: Qt.AlignLeft
        Layout.preferredWidth: Device.gu(5)
        Layout.preferredHeight: Device.gu(5)
        action: Action {
                icon.name: "go-first"
                onTriggered: {
                    if (cursorSwipeArea.selectionMode) {
                        fullScreenItem.selectStartOfLine();
                    } else {
                        fullScreenItem.moveToStartOfLine();
                    }
                }
            }
    }
    
    FloatingActionKey {
        id: startDocButton
        
        Layout.alignment: Qt.AlignLeft
        Layout.preferredWidth: Device.gu(5)
        Layout.preferredHeight: Device.gu(5)
        action: Action {
                icon.name: "go-top"
                onTriggered: {
                    if (cursorSwipeArea.selectionMode) {
                        fullScreenItem.selectStartOfDocument();
                    } else {
                        fullScreenItem.moveToStartOfDocument();
                    }
                }
            }
    }
    
    FloatingActionKey {
        id: doneButton
        
        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true
        Layout.minimumWidth: Device.gu(5)
        Layout.maximumWidth: Device.gu(20)
        Layout.preferredHeight: Device.gu(5)
        keyFeedback: false
        action: Action {
                text: qsTr("Done")
                icon.name: "ok"
                onTriggered: {
                    fullScreenItem.exitSwipeMode()
                }
            }
    }
        
        
        
    FloatingActionKey {
        id: endDocButton
        
        Layout.alignment: Qt.AlignRight
        Layout.preferredWidth: Device.gu(5)
        Layout.preferredHeight: Device.gu(5)
        action: Action {
                icon.name: "go-bottom"
                onTriggered: {
                    if (cursorSwipeArea.selectionMode) {
                        fullScreenItem.selectEndOfDocument();
                    } else {
                        fullScreenItem.moveToEndOfDocument();
                    }
                }
            }
    }
    
    FloatingActionKey {
        id: endLineButton
        
        Layout.alignment: Qt.AlignRight
        Layout.preferredWidth: Device.gu(5)
        Layout.preferredHeight: Device.gu(5)
        action: Action {
                icon.name: "go-last"
                onTriggered: {
                    if (cursorSwipeArea.selectionMode) {
                        fullScreenItem.selectEndOfLine();
                    } else {
                        fullScreenItem.moveToEndOfLine();
                    }
                }
            }
    }
}
