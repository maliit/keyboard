import QtQuick 2.9
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import MaliitKeyboard 2.0

import "keys/"

Item {
    anchors {
        fill: parent
        margins: Device.gu(1)
        topMargin: toolbar.height + Device.gu(1)
    }

    RowLayout {
        anchors {
            left: parent.left
            top: parent.top
        }

    FloatingActionKey {
        id: startLineButton
        
        Layout.alignment: Qt.AlignLeft
        Layout.preferredWidth: Device.gu(5)
        Layout.preferredHeight: Device.gu(5)
        action: Action {
                icon.name: "go-first-symbolic"
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
                icon.name: "go-top-symbolic"
                onTriggered: {
                    if (cursorSwipeArea.selectionMode) {
                        fullScreenItem.selectStartOfDocument();
                    } else {
                        fullScreenItem.moveToStartOfDocument();
                    }
                }
            }
    }
    }

    FloatingActionKey {
        id: doneButton
        
        width: Device.gu(5)
        height: width
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        keyFeedback: false
        action: Action {
            icon.name: "go-previous-symbolic"
            onTriggered: {
                fullScreenItem.exitSwipeMode()
            }
        }
    }
        

    RowLayout {
        anchors {
            right: parent.right
            top: parent.top
        }

    FloatingActionKey {
        id: endDocButton
        
        Layout.alignment: Qt.AlignRight
        Layout.preferredWidth: Device.gu(5)
        Layout.preferredHeight: Device.gu(5)
        action: Action {
                icon.name: "go-bottom-symbolic"
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
                icon.name: "go-last-symbolic"
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
}
