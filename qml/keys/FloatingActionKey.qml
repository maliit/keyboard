import QtQuick 2.9
import QtQuick.Controls 2.9

import MaliitKeyboard 2.0

Button {
    id: floatingActionKey
    
    /* design */
    property bool keyFeedback: true

    spacing: Device.gu(0.5)
    icon.name: action.icon.name
    display: action.text ? AbstractButton.TextBesideIcon : AbstractButton.IconOnly
    
    // Disable hover so that highlight doesn't stick on touch screens
    hoverEnabled: false

    onClicked: {
        if (keyFeedback) {
            Feedback.keyPressed();
        }
        fullScreenItem.timerSwipe.restart();
    }
}
