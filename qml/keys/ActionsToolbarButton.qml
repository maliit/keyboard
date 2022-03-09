import QtQuick 2.9
import QtQuick.Controls 2.9
import QtGraphicalEffects 1.0

import MaliitKeyboard 2.0

ToolButton {
    id: toolbarButton
    
    property bool fullLayout: false

    action: modelData
    spacing: Device.gu(0.5)
    icon.name: action.icon.name
    display: fullLayout ? AbstractButton.TextBesideIcon : AbstractButton.IconOnly
    
    onClicked: {
        Feedback.keyPressed();
        fullScreenItem.timerSwipe.restart();
    }
}
