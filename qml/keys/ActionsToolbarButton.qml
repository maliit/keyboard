import QtQuick 2.9
import QtQuick.Controls 2.9

import MaliitKeyboard 2.0

ToolButton {
    id: toolbarButton
    
    property bool fullLayout: false

    action: modelData
    spacing: Device.gu(0.5)
    icon.name: action.icon.name
    display: fullLayout ? AbstractButton.TextBesideIcon : AbstractButton.IconOnly
    
    // Disable hover so that highlight doesn't stick on touch screens
    hoverEnabled: false

    onClicked: {
        Feedback.keyPressed();
        fullScreenItem.timerSwipe.restart();
    }
}
