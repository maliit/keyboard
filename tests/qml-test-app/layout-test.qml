import QtQuick 2.0



import QtQuick 2.0
import Ubuntu.Components 0.1

MainView {
    id: root

    automaticOrientation: true

    Rectangle {
        anchors.fill: parent
        color: "lightblue"
    }

    Column {

        y: units.gu(3)
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: units.gu(1)

        anchors.margins: units.gu(1)

        Text {
            id: label
            text: "OSK Layout Test"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: units.gu(2)
            font.bold: true
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: units.gu(18);
            height: units.gu(6);

            anchors.leftMargin: units.gu(1)

            color: "white"

            TextInput {
                id: input;
                anchors.fill: parent
                color: "black"; selectionColor: "red"

                // Qt.ImhPreferNumbers
                inputMethodHints: Qt.ImhUrlCharactersOnly
                font.pixelSize: units.gu(3)
                font.bold: true
            }
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter;
            width: units.gu(18);
            height: units.gu(6);

            color: "white"

            TextInput {
                id: secondInput;
                anchors.fill: parent
                font.pixelSize: units.gu(3)
                inputMethodHints: Qt.ImhNone
            }
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter;
            width: units.gu(18);
            height: units.gu(6);

            color: "white"

            TextInput {
                anchors.fill: parent
                font.pixelSize: units.gu(3)
                inputMethodHints: Qt.ImhDigitsOnly
            }
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter;
            width: units.gu(18);
            height: units.gu(6);

            color: "white"

            TextInput {
                anchors.fill: parent
                font.pixelSize: units.gu(3)
                inputMethodHints: Qt.ImhDialableCharactersOnly
            }
        }


        Row {
            anchors.horizontalCenter: parent.horizontalCenter;

            Text {
                id: textRectangle

                font.pixelSize: units.gu(1)
                text: Qt.inputMethod.keyboardRectangle + " "
            }

            Rectangle {
                id: clickBehindOSKDetectedIndicator

                width: units.gu(18)
                height: units.gu(3)
                state: "default"

                Text {
                    id: clickBehindOSKDetectedIndicatorText
                    anchors.centerIn: parent
                    text: "clicked"
                    font.pixelSize: units.gu(2)
                    font.bold: true
                }

                states: [
                    State {
                        name: "default"
                        PropertyChanges {
                            target: clickBehindOSKDetectedIndicator
                            color: "gray"
                        }
                        PropertyChanges {
                            target: clickBehindOSKDetectedIndicatorText
                            color: "gray"
                        }
                    },
                    State {
                        name: "highlight"
                        PropertyChanges {
                            target: clickBehindOSKDetectedIndicator
                            color: "red"
                        }
                        PropertyChanges {
                            target: clickBehindOSKDetectedIndicatorText
                            color: "white"
                        }
                    }
                ]
            }

        }

        Text {
            id: textVisible

            anchors.horizontalCenter: parent.horizontalCenter;

            font.pixelSize: units.gu(2)
            text: "keyboard reports visible: " + Qt.inputMethod.visible
        }

    } // column

    Rectangle {
        id: fakeBrowserUrlBar
        width: keyboardRect.width
        height: units.gu(4)
        color: "orange"
        anchors.bottom: keyboardRect.top

        Text {
            visible: Qt.inputMethod.visible
            anchors.centerIn: parent
            font.pixelSize: units.gu(1)
            text: "browser url bar"
        }
    }

    Rectangle {
        id: keyboardRect

        width: Qt.inputMethod.keyboardRectangle.width
        height: Qt.inputMethod.keyboardRectangle.height

        anchors.bottom: parent.bottom
        color: " green"

        Text {
            visible: Qt.inputMethod.visible
            anchors.centerIn: parent
            font.pointSize: 24
            text: Qt.inputMethod.keyboardRectangle + " "
        }

        MouseArea {
            anchors.fill: parent
            onClicked: clickBehindOSKDetectedIndicator.visible = true
            onPressed: clickBehindOSKDetectedIndicator.state = "highlight"
            onReleased: clickBehindOSKDetectedIndicator.state = "default"
        }
    }
}
