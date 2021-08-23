import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import MaliitKeyboard 2.0

AbstractButton {
    id: floatingActionKey
    
    /* design */
    property string normalColor: Theme.charKeyColor
    property string pressedColor: Theme.charKeyPressedColor
    property bool keyFeedback: true
    
    onClicked: {
        if (keyFeedback) {
            fullScreenItem.keyFeedback();
        }
        fullScreenItem.timerSwipe.restart();
    }

    background: Rectangle {
        color: normalColor
        radius: Device.gu(0.5)

        Connections {
            target: floatingActionKey
            onPressedChanged: {
                if (target.pressed) {
                    color = pressedColor
                } else {
                    color = Qt.binding(function(){return normalColor})
                }
            }
        }

        Behavior on color {
            ColorAnimation {
                duration: 333
            }
        }
    }

    RowLayout {
        id: buttonsRow

        spacing: Device.gu(0.5)
        anchors {
            centerIn: parent
        }

        Image {
            id: icon

            Layout.preferredWidth: label.text ? Device.gu(2) : Device.gu(3)
            Layout.preferredHeight: Layout.preferredWidth
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            source: Theme.iconsPath + "/%1.png".arg(action.icon.name)
            fillMode: Image.PreserveAspectFit
            visible: false
        }

        ColorOverlay {
            Layout.preferredWidth: label.text ? Device.gu(2) : Device.gu(3)
            Layout.preferredHeight: Layout.preferredWidth
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            cached: true
            source: icon
            color: Theme.fontColor
            visible: action.icon.name != ""
        }

        Label {
            id: label

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            
            text: action.text
            renderType: Text.QtRendering
            font.weight: Font.Normal
            color: Theme.fontColor
        }
    }
}
