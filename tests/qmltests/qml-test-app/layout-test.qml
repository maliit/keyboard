import QtQuick 2.4
import Ubuntu.Components 1.3

MainView {
    id: root

    automaticOrientation: true

    property int inputWidth: units.gu( 14 )

    Rectangle {
        anchors.fill: parent
        color: "orange"
    }

    Column {
        id: c1
        y: units.gu(3)

        anchors.horizontalCenter: parent.horizontalCenter
        spacing: units.gu(1)

        Text {
            id: label
            text: "OSK Layout Test"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: units.gu(2)
            font.bold: true
        }

        /**
         * Url Characters
         */
        Row {
            spacing: units.gu(1)
            Item {
                width: inputWidth
                height: units.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhUrlCharactersOnly
                    font.pixelSize: units.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhUrlCharactersOnly"
                anchors.verticalCenter: parent
                font.pixelSize: units.gu(1.5)
            }
        }

        /**
         * Qt.ImhNone
         */
        Row {
            spacing: units.gu(1)
            Item {
                width: inputWidth
                height: units.gu(8);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextEdit {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhNone
                    font.pixelSize: units.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhNone, multiline"
                anchors.verticalCenter: parent
                font.pixelSize: units.gu(1.5)
            }
        }

        /**
         * Qt.ImhDigitsOnly
         */
        Row {
            spacing: units.gu(1)
            Item {
                width: inputWidth
                height: units.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhDigitsOnly
                    font.pixelSize: units.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhDigitsOnly"
                anchors.verticalCenter: parent
                font.pixelSize: units.gu(1.5)
            }
        }

        /**
         * Qt.ImhDialableCharactersOnly
         */
        Row {
            spacing: units.gu(1)
            Item {
                width: inputWidth
                height: units.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhDialableCharactersOnly
                    font.pixelSize: units.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhDialableCharactersOnly"
                anchors.verticalCenter: parent
                font.pixelSize: units.gu(1.5)
            }
        }

        /**
         * Qt.ImhEmailCharactersOnly
         */
        Row {
            spacing: units.gu(1)
            Item {
                width: inputWidth
                height: units.gu(4);

                Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                TextInput {
                    anchors.fill: parent
                    color: "black"; selectionColor: "red"

                    inputMethodHints: Qt.ImhEmailCharactersOnly
                    font.pixelSize: units.gu(3)
                    font.bold: false
                }
            }
            Text {
                text: "Qt.ImhEmailCharactersOnly"
                anchors.verticalCenter: parent
                font.pixelSize: units.gu(1.5)
            }
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
