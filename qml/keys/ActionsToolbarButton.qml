import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import MaliitKeyboard 2.0

AbstractButton {
    id: toolbarButton
    
    /* design */
    readonly property string normalColor: Theme.backgroundColor
    readonly property string pressedColor: Theme.charKeyPressedColor
    
    property bool fullLayout: false

    width: buttonsRow.width + Device.gu(2)

    action: modelData
    
    onClicked: {
        Feedback.keyPressed();
        fullScreenItem.timerSwipe.restart();
    }
    
    background: Rectangle {
        color: normalColor

        Connections {
            target: toolbarButton
            onPressedChanged:{
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
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
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
            visible: action.icon.name != ""
            color: Theme.fontColor
        }
        
        Label {
            id: label
            
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            
            visible: fullLayout
            font.pixelSize: Device.wordRibbonFontSize
            font.family: Theme.fontFamily
            font.weight: Font.Normal
            text: action.text
            elide: Text.ElideRight
            color: Theme.fontColor
        }
    }
}
