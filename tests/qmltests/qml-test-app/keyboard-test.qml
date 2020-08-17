

import QtQuick 2.4
import Ubuntu.Components 1.3

MainView {
    id: root

    automaticOrientation: true

    width: 8.0 * (18)
    height: 8.0 * (18)

    Rectangle {
        anchors.fill: parent
        color: "lightblue"
    }

    Column {

        y: 8.0 * (3)
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8.0 * (1)

        anchors.margins: 8.0 * (1)

        Text {
            id: label
            text: "OSK TestApp"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 8.0 * (2)
            font.bold: true
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 8.0 * (1)

            Button {
                color: "red"
                text: "show()"
                width: 8.0 * (18)
                onClicked: Qt.inputMethod.show()

            }

            Button {
                color: "red"
                text: "hide()"
                width: 8.0 * (18)
                onClicked: Qt.inputMethod.hide()
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter;

            text: qsTr("note: show/hide requires focus on a text input")
            font.pixelSize: 8.0 * (1)
            color: "gray"
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter

            spacing: 8.0 * (1)

            Rectangle {
                width: 8.0 * (18);
                height: firstInputButton.height;

                anchors.leftMargin: 8.0 * (1)

                color: "white"

                TextInput {
                    id: input;
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    // Qt.ImhPreferNumbers
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhUrlCharactersOnly | Qt.ImhNoPredictiveText
                    font.pixelSize: 8.0 * (3)
                    font.bold: true
                }
            }

            Button {
                id: firstInputButton
                color: "yellow"
                text: "Focus/Unfocus"
                width: 8.0 * (18)
                onClicked: input.focus = !input.focus
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;

            spacing: 8.0 * (1)

            Rectangle {
                width: 8.0 * (18);
                height: secondInputButton.height

                color: "white"

                TextInput {
                    id: secondInput;

                    anchors.fill: parent

                    font.pixelSize: 8.0 * (3)
                }
            }

            Button {
                id: secondInputButton
                color: "yellow"
                text: "Focus/Unfocus"
                width: 8.0 * (18)
                onClicked: secondInput.focus = !secondInput.focus
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;

            Text {
                id: textRectangle

                font.pixelSize: 8.0 * (1)
                text: Qt.inputMethod.keyboardRectangle + " "
            }

            Rectangle {
                id: clickBehindOSKDetectedIndicator

                width: 8.0 * (18)
                height: 8.0 * (3)
                state: "default"

                Text {
                    id: clickBehindOSKDetectedIndicatorText
                    anchors.centerIn: parent
                    text: "clicked"
                    font.pixelSize: 8.0 * (2)
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

            font.pixelSize: 8.0 * (2)
            text: "keyboard reports visible: " + Qt.inputMethod.visible
        }

    } // column

    Rectangle {
        id: fakeBrowserUrlBar
        width: keyboardRect.width
        height: 8.0 * (4)
        color: "orange"
        anchors.bottom: keyboardRect.top

        Text {
            visible: Qt.inputMethod.visible
            anchors.centerIn: parent
            font.pixelSize: 8.0 * (1)
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
