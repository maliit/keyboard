import QtQuick 2.4

import MaliitKeyboard 2.0

import Ubuntu.Components 1.3

MainView {
    id: root

    automaticOrientation: true

    property int inputWidth: Device.gu( 14 )

    Rectangle {
        anchors.fill: parent
        color: "orange"
    }

    Column {
        id: c1
        y: Device.gu(3)

        anchors.horizontalCenter: parent.horizontalCenter
        spacing: Device.gu(1)

        Text {
            id: label
            text: "OSK Layout Test"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Device.gu(2)
            font.bold: true
        }

        /**
         * Url Characters
         */
        Row {
            spacing: Device.gu(1)
            Item {
                width: inputWidth
                height: Device.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhUrlCharactersOnly
                    font.pixelSize: Device.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhUrlCharactersOnly"
                anchors.verticalCenter: parent
                font.pixelSize: Device.gu(1.5)
            }
        }

        /**
         * Qt.ImhNone
         */
        Row {
            spacing: Device.gu(1)
            Item {
                width: inputWidth
                height: Device.gu(8);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextEdit {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhNone
                    font.pixelSize: Device.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhNone, multiline"
                anchors.verticalCenter: parent
                font.pixelSize: Device.gu(1.5)
            }
        }

        /**
         * Qt.ImhDigitsOnly
         */
        Row {
            spacing: Device.gu(1)
            Item {
                width: inputWidth
                height: Device.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhDigitsOnly
                    font.pixelSize: Device.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhDigitsOnly"
                anchors.verticalCenter: parent
                font.pixelSize: Device.gu(1.5)
            }
        }

        /**
         * Qt.ImhDialableCharactersOnly
         */
        Row {
            spacing: Device.gu(1)
            Item {
                width: inputWidth
                height: Device.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhDialableCharactersOnly
                    font.pixelSize: Device.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhDialableCharactersOnly"
                anchors.verticalCenter: parent
                font.pixelSize: Device.gu(1.5)
            }
        }

        /**
         * Qt.ImhEmailCharactersOnly
         */
        Row {
            spacing: Device.gu(1)
            Item {
                width: inputWidth
                height: Device.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhEmailCharactersOnly
                    font.pixelSize: Device.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhEmailCharactersOnly"
                anchors.verticalCenter: parent
                font.pixelSize: Device.gu(1.5)
            }
        }
    } // column

    Rectangle {
        id: fakeBrowserUrlBar
        width: keyboardRect.width
        height: Device.gu(4)
        color: "orange"
        anchors.bottom: keyboardRect.top

        Text {
            visible: Qt.inputMethod.visible
            anchors.centerIn: parent
            font.pixelSize: Device.gu(1)
            text: "app bottom"
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
