import QtQuick 2.4
import QtTest 1.0 // for TestCase
import Lomiri.Components 1.3
import MaliitTestSupport 1.0

/*
 *  qmltestrunner -import maliit-keyboard/tests/qml-api-tests/imports/ -input maliit-keyboard/tests/qml-api-tests/tst_inputMethodHints.qml
 *
 *  Note: if tests fail, try to increase asyncDelay
 */

Item {
    id: mainWindow

    width: 1280
    height: 720

    anchors.margins: 60

    /**
     maliitTestHelper.localeName(Qt.inputMethod.locale)
     maliitTestHelper.localeUiLanguages(Qt.inputMethod.locale)
     */

    MaliitTestHelper {
        id: maliitTestHelper
    }

    Rectangle
    {
        anchors.fill: parent
        color: "red"
    }

    TextField {
        id: unusedTextField
        anchors {
            top: mainWindow.top
            horizontalCenter: parent.horizontalCenter
            topMargin: 50
        }
        inputMethodHints: Qt.ImhUrlCharactersOnly
    }

    TextField {
        id: textField
        anchors {
            top: unusedTextField.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 50
        }
        inputMethodHints: Qt.ImhUrlCharactersOnly
    }

    TextField {
        id: noPredictiveTextField
        anchors {
            top: textField.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 50
        }
        inputMethodHints: Qt.ImhNoPredictiveText
    }

    TestCase {
        id: inputMethodHintsTest
        name: "inputMethodHints"

        // We must wait until the window is shown because we're going to do mouse interaction
        when: windowShown

        function initTestCase() {
        }

        function test_1_oskShownHide()
        {
            compare(Qt.inputMethod.visible, false)

            click(textField)

            tryCompare(Qt.inputMethod, "visible", true)

            wait(500)

            // TODO get actual values from MaliitTestHelper, e.g height 608
            verify(Qt.inputMethod.keyboardRectangle.height > 0)

            textField.focus = false
            compare(textField.focus, false)

            wait(500)

            tryCompare(Qt.inputMethod, "visible", false)
            tryCompare(Qt.inputMethod.keyboardRectangle, "height", 0)
        }

        function test_2_noPredictiveText()
        {
            textField.focus = true

            click(textField)

            tryCompare(Qt.inputMethod, "visible", true)
            wait(500)
            verify(Qt.inputMethod.keyboardRectangle.height > 0)
            tryCompare(Qt.inputMethod.keyboardRectangle, "height", 796)

            textField.focus = false
            noPredictiveTextField.focus = true

            tryCompare(Qt.inputMethod.keyboardRectangle, "height", 608)
        }

        /* helper */

        function click(target) {
            mouseClick(target, (target).width / 2, (target).height / 2)
            keyClick(Qt.Key_5)
        }

    }

}
