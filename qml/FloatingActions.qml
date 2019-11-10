import QtQuick 2.9
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.3
import "keys/"

RowLayout {
    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
        margins: units.gu(1)
        topMargin: toolbar.height + units.gu(1)
    }
    
    FloatingActionKey {
        id: startLineButton
        
        Layout.alignment: Qt.AlignLeft
        Layout.preferredWidth: units.gu(5)
        Layout.preferredHeight: units.gu(5)
        action: Action {
                iconName: "go-first"
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
        
        iconRotation: 90
        Layout.alignment: Qt.AlignLeft
        Layout.preferredWidth: units.gu(5)
        Layout.preferredHeight: units.gu(5)
        action: Action {
                iconName: "go-first"
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
        Layout.minimumWidth: units.gu(5)
        Layout.maximumWidth: units.gu(20)
        Layout.preferredHeight: units.gu(5)
        keyFeedback: false
        action: Action {
                text: i18n.tr("Done")
                iconName: "ok"
                onTriggered: {
                    fullScreenItem.exitSwipeMode()
                }
            }
    }
        
        
        
    FloatingActionKey {
        id: endDocButton
        
        iconRotation: 90
        Layout.alignment: Qt.AlignRight
        Layout.preferredWidth: units.gu(5)
        Layout.preferredHeight: units.gu(5)
        action: Action {
                iconName: "go-last"
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
        Layout.preferredWidth: units.gu(5)
        Layout.preferredHeight: units.gu(5)
        action: Action {
                iconName: "go-last"
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
