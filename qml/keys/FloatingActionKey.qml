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
    
    onClicked: {
        if (keyFeedback) {
            Feedback.keyPressed();
        }
        fullScreenItem.timerSwipe.restart();
    }
}
