import os

from testtools.matchers import IsInstance, Equals
from tempfile import mktemp
from textwrap import dedent

from autopilot.testcase import AutopilotTestCase
from autopilot.input import Pointer, Touch
from autopilot.matchers import Eventually

from maliit_keyboard.emulators.osk import (
    OSK,
    KB_STATE_DEFAULT,
    KB_STATE_SHIFTED,
)


class OSKeyboardTests(AutopilotTestCase):
    def setUp(self):
        super(OSKeyboardTests, self).setUp()
        self.pointer = Pointer(Touch.create())

    # This does 2 things, launches and clicks waiting for the keyboard. Split
    # this up.
    def launch_test_input_area(self):
        self.app = self._launch_simple_input()
        text_rectangle = self.app.select_single(
            "QQuickRectangle",
            objectName='inputRectangle'
        )
        text_area = self.app.select_single("QQuickTextInput")
        self.pointer.click_object(text_rectangle)
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)
        self.assertThat(keyboard.is_available, Eventually(Equals(True)))

        return text_area

    def _start_qml_script(self, script_contents):
        """Launch a qml script."""
        qml_path = mktemp(suffix='.qml')
        open(qml_path, 'w').write(script_contents)
        self.addCleanup(os.remove, qml_path)

        return self.launch_test_application(
            "qmlscene",
            qml_path,
            app_type='qt',
        )

    def _launch_simple_input(self):
        simple_script = dedent("""
        import QtQuick 2.0
        import Ubuntu.Components 0.1

        Rectangle {
            id: window
            objectName: "windowRectangle"
            //width: 500
            //height: 500
            color: "lightgrey"

            Rectangle {
                objectName: "inputRectangle"
                width: parent.width
                height: parent.height / 2
                color: "white"
                TextInput {
                    id: input;
                    anchors.fill: parent
                    color: "black";
                    selectionColor: "red"

        // Veebers: This shouldn't have NoAutoUppercase etc. perhaps should only have noPredictive.
                    // Qt.ImhPreferNumbers
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhUrlCharactersOnly | Qt.ImhNoPredictiveText
                    font.pixelSize: units.gu(3)
                    font.bold: false
                }
            }
        }

        """)

        return self._start_qml_script(simple_script)


class OSKeyboardTestsAccess(OSKeyboardTests):

    def test_can_create_keyboard(self):
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)
        self.assertThat(keyboard, IsInstance(OSK))

    def test_keyboard_is_available(self):
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)
        app = self._launch_simple_input()
        text_rectangle = app.select_single(
            "QQuickRectangle",
            objectName='inputRectangle'
        )

        self.pointer.click_object(text_rectangle)

        self.assertThat(keyboard.is_available, Eventually(Equals(True)))


class OSKeyboardTypingTests(OSKeyboardTests):

    scenarios = [
        ('lower_alpha', dict(input='abcdefghijklmnopqrstuvwxyz')),
        ('upper_alpha', dict(input='ABCDEFGHIJKLMNOPQRSTUVWXYZ')),
        ('numeric', dict(input='0123456789')),
        ('punctuation', dict(input='`~!@#$%^&*()_-+={}[]|\\:;"\'<>,.?/'))
    ]

    def test_can_type_string(self):
        text_area = self.launch_test_input_area()
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(self.input)

        self.assertThat(text_area.text, Eventually(Equals(self.input)))


class OSKeyboardStateChanges(OSKeyboardTests):

    def test_keyboard_layout_starts_shifted(self):
        """When first launched the keyboard state must be
        shifted/capitalised.

        """
        text_area = self.launch_test_input_area()
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)

        self.assertThat(
            keyboard.keyboard.layoutState,
            Eventually(Equals(KB_STATE_SHIFTED))
        )

    def test_shift_latch(self):
        """Double tap of the shift key must lock it 'On' until the shift key
        tapped again.

        Normally hitting shift then a letter reverts from the shifted state
        back to the default. If double clicked it should stay in the shifted
        until the shift key is clicked again.

        """
        text_area = self.launch_test_input_area()
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('abc')
        keyboard._press_special_key('SHIFT')
        keyboard._press_special_key('SHIFT')
        keyboard.type('S')

        self.assertThat(
            keyboard.keyboard.layoutState,
            Eventually(Equals(KB_STATE_SHIFTED))
        )
        self.assertThat(text_area.text, Eventually(Equals('abcS')))

    def test_shift_state_returns_to_default_after_letter_typed(self):
        """Pushing shift and then typing an uppercase letter must automatically
        shift the keyboard back into the default state.

        """
        text_area = self.launch_test_input_area()
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)

        # Normally, type and (press_key) take care of shifting into the correct
        # state, we do it manually here as that's what we're testing.
        keyboard.type('abc')
        keyboard._press_special_key('SHIFT')
        keyboard.type('A')

        # Once the capital letter has been typed, we must be able to access the
        # lowercase letters, otherwise it's not in the correct state.
        self.assertThat(
            keyboard.keyboard.layoutState,
            Eventually(Equals(KB_STATE_DEFAULT))
        )

        self.assertThat(text_area.text, Eventually(Equals('abcA')))

    # Note: this is a failing test.
    # Note: Based of UX design spec.
    def test_shift_state_entered_after_fullstop(self):
        """After typing a fullstop the keyboard state must automatically
        enter the shifted state.

        """
        text_area = self.launch_test_input_area()
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)

        keyboard.type("abc.")

        self.assertThat(
            text_area.text,
            Eventually(Equals("abc."))
        )

        self.assertThat(
            keyboard.keyboard.layoutState,
            Eventually(Equals(KB_STATE_SHIFTED))
        )

    def test_switching_between_states(self):
        text_area = self.launch_test_input_area()
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(
            'This shouldd\b type & display different  \bCHARs (characters).'
        )

        expected = "This should type & display different CHARs (characters)."
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class OSKeyboardInputTypeStateChange(OSKeyboardTests):
    scenarios = [
        ("Url", dict(hints=['Qt.ImhUrlCharactersOnly'], expected_activeview=0)),
        ("Password", dict(hints=['Qt.ImhHiddenText', 'Qt.ImhSensitiveData'], expected_activeview=0)),
        ("Email", dict(hints=['Qt.ImhEmailCharactersOnly'], expected_activeview=0)),
        ("Number", dict(hints=['Qt.ImhFormattedNumbersOnly'], expected_activeview=0)),
        ("Telephone", dict(hints=['Qt.ImhDigitsOnly'], expected_activeview=0))
    ]

    # This does 2 things, launches and clicks waiting for the keyboard. Split
    # this up.
    def launch_test_input_area2(self, input_hints=None):
        self.app = self._launch_input_with_hints(input_hints)
        text_rectangle = self.app.select_single(
            "QQuickRectangle",
            objectName='inputRectangle'
        )
        text_area = self.app.select_single("QQuickTextInput")
        self.pointer.click_object(text_rectangle)
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)
        self.assertThat(keyboard.is_available, Eventually(Equals(True)))

        return text_area

    # Perhaps this could just be resolved into the main one, i.e. if nothing is
    # passed then just have an empty string, otherwise produce the string
    # required to get this working.
    def _launch_input_with_hints(self, input_hints=None):

        if input_hints is None:
            extra_script = ""
        else:
            extra_script = "|".join(input_hints)

        simple_script = dedent("""
        import QtQuick 2.0
        import Ubuntu.Components 0.1

        Rectangle {
            id: window
            objectName: "windowRectangle"
            //width: 500
            //height: 500
            color: "lightgrey"

            Rectangle {
                objectName: "inputRectangle"
                width: parent.width
                height: parent.height / 2
                color: "white"
                TextInput {
                    id: input;
                    anchors.fill: parent
                    color: "black";
                    selectionColor: "red"

                    inputMethodHints: %s
                    font.pixelSize: units.gu(3)
                    font.bold: false
                }
            }
        }

        """ % extra_script)

        return self._start_qml_script(simple_script)

    def test_keyboard_layout(self):
        """The OSK must respond to the input type and change to be the correct
        state.

        """
        text_area = self.launch_test_input_area2(self.hints)
        keyboard = OSK()
        self.addCleanup(keyboard.dismiss)

        self.assertThat(
            keyboard.layoutState,
            Eventually(Equals(self.expected_activeview))
        )
