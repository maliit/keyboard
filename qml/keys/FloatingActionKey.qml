import QtQuick 2.9
import Ubuntu.Components 1.3

AbstractButton {
    id: floatingActionKey
    
    /* design */
    property string normalColor: fullScreenItem.theme.charKeyColor
    property string pressedColor: fullScreenItem.theme.charKeyPressedColor
    property bool keyFeedback: true
    
    //TODO: Remove if ever more appropriate icons become available that don't need rotation.
    property alias iconRotation: icon.rotation
    
    onClicked: {
        if (keyFeedback) {
            fullScreenItem.keyFeedback();
        }
        fullScreenItem.timerSwipe.restart();
    }

    style: Rectangle {
        color: normalColor
        radius: units.gu(0.5)

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
                easing: UbuntuAnimation.StandardEasing
                duration: UbuntuAnimation.BriskDuration
            }
        }
    }

    Row {
        id: buttonsRow

        spacing: units.gu(0.5)
        anchors {
            centerIn: parent
        }

        Icon {
            id: icon

            name: action.iconName
            width: label.text ? units.gu(2) : units.gu(3)
            height: width
            visible: action.iconName
            color: fullScreenItem.theme.fontColor
        }

        Label {
            id: label

            text: action.text
            renderType: Text.QtRendering
            font.weight: Font.Normal
            color: fullScreenItem.theme.fontColor
        }
    }
}
