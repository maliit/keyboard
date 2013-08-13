
import QtQuick 2.0
import Ubuntu.Components 0.1

Rectangle {
    id: window
    objectName: "windowRectangle"
    //width: 500
    //height: 500
    color: "lightgrey"

    Rectangle {
        id: inputRectangle
        width: parent.width
        height: parent.height / 2
        color: "white"
        TextInput {
            id: input;
            anchors.fill: parent
            color: "black";
            selectionColor: "red"

            // Qt.ImhPreferNumbers
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhUrlCharactersOnly | Qt.ImhNoPredictiveText
            font.pixelSize: units.gu(3)
            font.bold: false
        }
    }
}
