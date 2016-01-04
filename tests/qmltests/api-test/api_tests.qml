import QtQuick 2.4
import QtTest 1.0
import Ubuntu.Components 1.3

/*
  tests for API exposed to the app in QML
 */

MainView {

    TextField {
        id: textField
    }

    TestCase {
        when: windowShown

        function test_keyboardRect() {
            compare(Qt.inputMethod.keyboardRectangle.height, 0)

            sleep(3000)
//            Qt.inputMethod.show();
            textField.forceActiveFocus()

            sleep(3000)

            print("keyboard rect is", Qt.inputMethod.keyboardRectangle)
            verify(Qt.inputMethod.keyboardRectangle.height > 0)

            Qt.inputMethod.hide();

            sleep(1000)

            compare(Qt.inputMethod.keyboardRectangle.height, 0)

        }
    }
}
