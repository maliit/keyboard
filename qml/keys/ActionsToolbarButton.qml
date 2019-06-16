import QtQuick 2.9
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.3
import "key_constants.js" as UI

AbstractButton {
    id: toolbarButton
    
    /* design */
    property string normalColor: fullScreenItem.theme.backgroundColor
    property string pressedColor: fullScreenItem.theme.charKeyPressedColor
    
    property bool fullLayout: false
    
    anchors {
        top: parent ? parent.top : undefined
        bottom: parent ? parent.bottom : undefined
    }
    width: buttonsRow.width + units.gu(2)
    
    action: modelData
    
    onClicked: {
        fullScreenItem.keyFeedback();
        fullScreenItem.timerSwipe.restart();
    }
    
    style: Rectangle {
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
                easing: UbuntuAnimation.StandardEasing
                duration: UbuntuAnimation.BriskDuration
            }
        }
    }

    RowLayout {
        id: buttonsRow

        spacing: units.gu(0.5)
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
        
        Icon {
            id: icon

            Layout.preferredWidth: label.text ? units.gu(2) : units.gu(3)
            Layout.preferredHeight: Layout.preferredWidth
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            
            name: action.iconName
            visible: action.iconName
            color: fullScreenItem.theme.fontColor
        }
        
        Label {
            id: label
            
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            
            visible: fullLayout
            font.pixelSize: units.dp(fullScreenItem.tablet ? UI.tabletWordRibbonFontSize : UI.phoneWordRibbonFontSize)
            font.family: UI.fontFamily
            font.weight: Font.Normal
            text: action.text
            elide: Text.ElideRight
            color: fullScreenItem.theme.fontColor
        }
    }
}
