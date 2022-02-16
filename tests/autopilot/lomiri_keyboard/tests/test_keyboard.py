# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
#
# Lomiri Keyboard Test Suite
# Copyright (C) 2013, 2015 Canonical
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import shutil
import subprocess
import atexit

from testtools.matchers import Equals
import tempfile
from textwrap import dedent
from time import sleep

from autopilot.testcase import AutopilotTestCase
from autopilot.input import Pointer, Touch
from autopilot.matchers import Eventually
from autopilot.platform import model
from lomiriuitoolkit import base

from lomiri_keyboard.emulators.keyboard import Keyboard
from lomiri_keyboard.emulators.keypad import KeyPadState

from gi.repository import Gio

import logging


logger = logging.getLogger(__name__)


class LomiriKeyboardTests(AutopilotTestCase):

    @classmethod
    def setUpClass(cls):
        # FIXME: This is a work around re: lp:1238417 ####
        if model() != "Desktop":
            from autopilot.input import _uinput
            _uinput._touch_device = _uinput.create_touch_device()

    def setUp(self):
        if model() == "Desktop":
            self.skipTest("Lomiri Keyboard tests only run on device.")
        super(LomiriKeyboardTests, self).setUp()
        self.set_test_settings()
        sleep(5)  # Have to give time for gsettings change to propogate
        self.pointer = Pointer(Touch.create())

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_strv(
            "enabled-languages", ["en", "es", "de",
                                  "zh-hans", "zh-hant", "emoji"])
        gsettings.set_string("active-language", "en")
        gsettings.set_string("previous-language", "es")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def launch_test_input_area(self, label="", input_hints=None):
        self.app = self._launch_simple_input(label, input_hints)
        text_area = self.app.select_single("TextArea")

        return text_area

    def ensure_focus_on_input(self, input_area):
        self.pointer.click_object(input_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)
        self.assertThat(keyboard.is_available, Eventually(Equals(True)))

    def _start_qml_script(self, script_contents):
        """Launch a qml script."""
        qml_path = tempfile.mktemp(suffix='.qml')
        open(qml_path, 'w').write(script_contents)
        self.addCleanup(os.remove, qml_path)

        # Use installed desktop file so that Mir allows us to connect with
        # our test QML apps
        desktop_file = "/usr/share/applications/lomiri-keyboard-tester.desktop"
        return self.launch_test_application(
            base.get_qmlscene_launch_command(),
            qml_path,
            '--desktop_file_hint=%s' % desktop_file,
            app_type='qt',
        )

    def _write_test_desktop_file(self):
        desktop_file_dir = self.get_local_desktop_file_directory()
        if not os.path.exists(desktop_file_dir):
            os.makedirs(desktop_file_dir)
        with tempfile.NamedTemporaryFile(
            suffix='.desktop',
            dir=desktop_file_dir,
            delete=False
        ) as desktop_file:
            desktop_file.write(
                "[Desktop Entry]\n"
                "Type=Application\n"
                "Exec=Not important\n"
                "Path=Not important\n"
                "Name=Test app\n"
                "Icon=Not important"
            )
        self.addCleanup(os.remove, desktop_file.name)
        return desktop_file.name

    def get_local_desktop_file_directory(self):
        return os.path.join(
            os.getenv('HOME'),
            '.local',
            'share',
            'applications'
        )

    def _launch_simple_input(self, label="", input_hints=None):
        extra_script = "undefined"
        if input_hints is not None:
            extra_script = "|".join(input_hints)

        simple_script = dedent("""
        import QtQuick 2.4
        import Lomiri.Components 1.3

        Rectangle {
            id: window
            objectName: "windowRectangle"
            color: "lightgrey"

            Text {
                id: inputLabel
                text: "%(label)s"
                font.pixelSize: Device.gu(3)
                anchors {
                    left: input.left
                    top: parent.top
                    topMargin: 25
                    bottomMargin: 25
                }
            }

            TextArea {
                id: input;
                objectName: "input"
                anchors.centerIn: parent
                inputMethodHints: %(input_method)s
                autoSize: true
            }
        }

        """ % {'label': label, 'input_method': extra_script})

        return self._start_qml_script(simple_script)


class LomiriKeyboardTestsAccess(LomiriKeyboardTests):

    def test_keyboard_is_available(self):
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)
        app = self._launch_simple_input(input_hints=['Qt.ImhNoPredictiveText'])
        text_rectangle = app.select_single("TextArea")

        self.pointer.click_object(text_rectangle)

        self.assertThat(keyboard.is_available, Eventually(Equals(True)))


class LomiriKeyboardTypingTests(LomiriKeyboardTests):

    scenarios = [
        (
            'lower_alpha',
            dict(
                label="Lowercase",
                input='abcdefghijklmnopqrstuvwxyz'
            )
        ),
        (
            'upper_alpha',
            dict(
                label="Uppercase",
                input='ABCDEFGHIJKLMNOPQRSTUVWXYZ'
            )
        ),
        (
            'numeric',
            dict(
                label="Numeric",
                input='0123456789'
            )
        ),
        (
            # Currently the en_us layout doesn't have ", but has \u201c and
            # \u201d
            'punctuation',
            dict(
                label="Punctuation",
                input=u'`~!@#$%^&*()_-+={}[]|\\:;\'<>,.?/\u201c'
            )
        )
    ]

    def test_can_type_string(self):
        text_area = self.launch_test_input_area(
            label=self.label, input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(self.input)
        self.assertThat(text_area.text, Eventually(Equals(self.input)))


class LomiriKeyboardKeyMappingTests(LomiriKeyboardTests):

    scenarios = [
        (
            'backspace',
            dict(
                starting_text='Start text',
                input_sequence=['Backspace', '\b', 'backspace'],
                expected_text=['Start tex', 'Start te', 'Start t']
            )
        ),
        (
            'space',
            dict(
                starting_text='',
                input_sequence=['Space', ' ', 'space'],
                expected_text=[' ', '  ', '   ']
            )
        ),
        (
            'return',
            dict(
                starting_text='',
                input_sequence=['Enter', '\n', 'return'],
                expected_text=['\n', '\n\n', '\n\n\n']
            )
        )
    ]

    def setUp(self):
        super().setUp()
        self._disable_double_space_full_stop()

    def _disable_double_space_full_stop(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_boolean("double-space-full-stop", False)

    def test_can_type_using_key_mapping(self):
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(self.starting_text)
        for key, result in zip(self.input_sequence, self.expected_text):
            keyboard.press_key(key)
            self.assertThat(text_area.text, Eventually(Equals(result)))


class LomiriKeyboardStateKeyMappingTests(LomiriKeyboardTests):

    scenarios = [
        (
            'Shift',
            dict(
                key='Shift',
                expected_state=KeyPadState.SHIFTED,
                expected_keypad_name='CHARACTERS'
            )
        ),
        (
            'SHIFT',
            dict(
                key='SHIFT',
                expected_state=KeyPadState.SHIFTED,
                expected_keypad_name='CHARACTERS'
            )
        ),
        (
            'shift',
            dict(
                key='shift',
                expected_state=KeyPadState.SHIFTED,
                expected_keypad_name='CHARACTERS'
            )
        ),
        (
            'ABC',
            dict(
                key='ABC',
                expected_state=KeyPadState.NORMAL,
                expected_keypad_name='SYMBOLS'
            )
        ),
        (
            '?123',
            dict(
                key='?123',
                expected_state=KeyPadState.NORMAL,
                expected_keypad_name='SYMBOLS'
            )
        ),
        (
            'symbols',
            dict(
                key='symbols',
                expected_state=KeyPadState.NORMAL,
                expected_keypad_name='SYMBOLS'
            )
        )
    ]

    def _assert_keypad_name_and_state(self, keyboard, name, state):
        self.assertThat(
            keyboard._current_keypad_name,
            Eventually(Equals(name))
        )
        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(state))
        )

    def test_can_type_using_state_key_mapping(self):
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText', 'Qt.ImhNoAutoUppercase'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)
        self._assert_keypad_name_and_state(keyboard,
                                           'CHARACTERS', KeyPadState.NORMAL)
        keyboard.press_key(self.key)
        self._assert_keypad_name_and_state(keyboard,
                                           self.expected_keypad_name,
                                           self.expected_state)


class LomiriKeyboardStateChanges(LomiriKeyboardTests):

    def test_keyboard_layout_starts_shifted(self):
        """When first launched the keyboard state must be
        shifted/capitalised.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.SHIFTED))
        )

    def test_shift_latch(self):
        """Double tap of the shift key must lock it 'On' until the shift key
        tapped again.

        Normally hitting shift then a letter reverts from the shifted state
        back to the default. If double clicked it should stay in the shifted
        until the shift key is clicked again.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('abc')
        keyboard.press_key('shift')
        keyboard.press_key('shift', True)

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.CAPSLOCK))
        )

    def test_capslock_stays_on(self):
        """Capslock should remain on when typing text.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.press_key('shift')
        keyboard.press_key('shift', True)
        keyboard.press_key('A', True)
        keyboard.press_key('B', True)
        keyboard.press_key('C', True)

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.CAPSLOCK))
        )

    # Note: based on UX design doc
    def test_shift_state_returns_to_default_after_letter_typed(self):
        """Pushing shift and then typing an uppercase letter must automatically
        shift the keyboard back into the default state.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        # Normally, type and (press_key) take care of shifting into the correct
        # state, we do it manually here as that's what we're testing.
        keyboard.type('abc')
        keyboard.press_key('SHIFT')
        keyboard.type('A')

        # Once the capital letter has been typed, we must be able to access the
        # lowercase letters, otherwise it's not in the correct state.
        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.NORMAL))
        )

        self.assertThat(text_area.text, Eventually(Equals('abcA')))

    def test_shift_state_entered_after_fullstop(self):
        """After typing a fullstop followed by a space the keyboard state must
        automatically enter the shifted state.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type("abc. ")

        self.assertThat(
            text_area.text,
            Eventually(Equals("abc. "))
        )

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.SHIFTED))
        )

    def test_shift_state_left_after_deleting_fullstop(self):
        """After deleting a fullstop the keyboard should return to the normal
        state.
        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type("Hello my friend. \b\b")

        self.assertThat(
            text_area.text,
            Eventually(Equals("Hello my friend"))
        )

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.NORMAL))
        )

    def test_switching_between_states(self):
        """The user must be able to type many different characters including
        spaces and backspaces.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(
            'abc gone\b\b & \b ABC (123)'
        )

        expected = "abc go & ABC (123)"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_visibility_reporting(self):
        """The keyboard should only report visibility changes once.

        """

        # Test bug #1436076 which only occurs after maliit is restarted
        subprocess.check_call(['initctl', 'set-env', 'QT_LOAD_TESTABILITY=1'])
        subprocess.check_call(['restart', 'maliit-server'])
        sleep(10)

        qml = dedent("""
        import QtQuick 2.4
        import Lomiri.Components 1.3

        Rectangle {
            id: window
            objectName: "windowRectangle"
            color: "lightgrey"

            TextField {
                id: input;
                objectName: "input"
                anchors.centerIn: parent
                property int visibilityChangeCount: 0
            }

            Connections {
                target: Qt.inputMethod
                onVisibleChanged: {
                    input.visibilityChangeCount++;
                }
            }
        }

        """)
        app = self._start_qml_script(qml)
        text_area = app.select_single(objectName='input')
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        keyboard.dismiss()

        self.assertThat(
            text_area.visibilityChangeCount,
            Eventually(Equals(2))
        )


class LomiriKeyboardInputTypeStateChange(LomiriKeyboardTests):

    scenarios = [
        (
            "Url",
            dict(
                label="Url",
                hints=['Qt.ImhUrlCharactersOnly'],
                expected_activeview="url",
                text="google.com"
            )
        ),
        (
            "Email",
            dict(
                label="Email",
                hints=['Qt.ImhEmailCharactersOnly'],
                expected_activeview="email",
                text="test.user@example.com"
            )
        ),
        (
            "Number",
            dict(
                label="Number",
                hints=['Qt.ImhFormattedNumbersOnly'],
                expected_activeview="number",
                text="3.14159"
            )
        ),
        (
            "Telephone",
            dict(
                label="Telephone",
                hints=['Qt.ImhDigitsOnly'],
                expected_activeview="number",
                text="01189998819991197253"
            )
        ),
    ]

    # Note: based on UX design doc
    def test_keyboard_layout(self):
        """The Keyboard must respond to the input type and change to be the
        correct state.

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        self.assertThat(
            keyboard.keyboard.layoutId,
            Eventually(Equals(self.expected_activeview))
        )

        if self.text[-4:] == ".com":
            keyboard.type(self.text[:-4])
            keyboard.press_key(".com")
        else:
            keyboard.type(self.text)

        self.assertThat(
            text_area.text,
            Eventually(Equals(self.text))
        )


class LomiriKeyboardAdvancedFeatures(LomiriKeyboardTests):

    def test_double_space_fullstop(self):
        """After tapping space twice a fullstop should be entered.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('This is a test  ')

        expected = "This is a test. "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_override(self):
        """After typing 'i' followed by a space it should get auto-corrected
        to 'I' via the override mechanism.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('i i i ')

        expected = "I I I "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_double_space_single_character(self):
        """Spaces should be auto-inserted after double pressing space following
        a single character.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Test i  ')

        expected = "Test I. "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_autocomplete(self):
        """Tapping space in a field that supports auto-complete should
           complete a word.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Pic ')

        expected = "Picture "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_restore_preedit(self):
        """Pressing delete after autocompleting a word should restore
           the original preedit state.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Helfn')

        sleep(2)

        keyboard.type(' ')

        expected = "Helen "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

        keyboard.type('\b ')

        expected = "Helfn "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_long_press(self):
        """Long pressing a key should enter the default extended character.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.press_key('t', long_press=True)

        expected = "5"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_repeated_long_press(self):
        """The default key should stay in the middle after each long press.

        """

        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.press_key('u', long_press=True)
        keyboard.press_key('u', long_press=True)
        keyboard.press_key('u', long_press=True)

        expected = "777"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_extended_punctuation(self):
        """The characters -_!?+%#/ and ";:'@&() should be available as
        extended keys from the . and , keys.

        """

        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        gu = float(os.environ.get('GRID_UNIT_PX', 8))

        # The extended key positions aren't accessible until the
        # drag has already started, so we need to provide their
        # offsets manually based on the known extended key cell
        # width
        keyboard.press_key('.', slide_offset=-17.5 * gu)
        keyboard.press_key('.', slide_offset=-14 * gu)
        keyboard.press_key('.', slide_offset=-10.5 * gu)
        keyboard.press_key('.', slide_offset=-7.5 * gu)
        keyboard.press_key('.', slide_offset=-3.5 * gu)
        keyboard.press_key('.', slide_offset=1)
        keyboard.press_key('.', slide_offset=3.5 * gu)
        keyboard.press_key('.', slide_offset=7 * gu)

        keyboard.press_key(',', slide_offset=-10.5 * gu)
        keyboard.press_key(',', slide_offset=-7 * gu)
        keyboard.press_key(',', slide_offset=-3.5 * gu)
        keyboard.press_key(',', slide_offset=-1 * gu)
        keyboard.press_key(',', slide_offset=3.5 * gu)
        keyboard.press_key(',', slide_offset=7 * gu)
        keyboard.press_key(',', slide_offset=10.5 * gu)
        keyboard.press_key(',', slide_offset=14 * gu)

        expected = "-_!?+%#/\";:'@&()"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_single_quotes(self):
        """Single quotes placed around a word shouldn't get removed by
        autocomplete.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type("'hello' 'there' ")

        expected = "'hello' 'there' "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardPinyin(LomiriKeyboardTests):

    scenarios = [
        (
            "Url",
            dict(
                label="Url",
                hints=['Qt.ImhUrlCharactersOnly'],
                expected_activeview="url"
            )
        ),
        (
            "Email",
            dict(
                label="Email",
                hints=['Qt.ImhEmailCharactersOnly'],
                expected_activeview="email"
            )
        ),
        (
            "FreeText",
            dict(
                label="FreeText",
                hints=None,
                expected_activeview="freetext"
            )
        ),
        (
            "NoPredictiveText",
            dict(
                label="NoPredictiveText",
                hints=['Qt.ImhNoPredictveText'],
                expected_activeview="freetext"
            )
        ),
    ]

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_string("active-language", "zh-hans")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def test_pinyin(self):
        """Switching to simplified Chinese should result in pinyin characters
        being entered via autocomplete regardless of layout or prediction
        being disabled.

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('pinyin ')

        expected = "拼音"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_fullstop(self):
        """Full stop shouldn't have space added after it in pinyin mode.

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('pinyin.pinyin ')

        expected = "拼音.拼音"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_auto_punctuation(self):
        """A chinese full-stop character should be entered after space has
        been pressed three times (once to complete the character, once more
        to insert a space and then again to produce a full-stop.

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('pinyin   ')

        expected = "拼音。 "

        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardChewing(LomiriKeyboardTests):

    scenarios = [
        (
            "Url",
            dict(
                label="Url",
                hints=['Qt.ImhUrlCharactersOnly'],
                expected_activeview="url"
            )
        ),
        (
            "Email",
            dict(
                label="Email",
                hints=['Qt.ImhEmailCharactersOnly'],
                expected_activeview="email"
            )
        ),
        (
            "FreeText",
            dict(
                label="FreeText",
                hints=None,
                expected_activeview="freetext"
            )
        ),
        (
            "NoPredictiveText",
            dict(
                label="NoPredictiveText",
                hints=['Qt.ImhNoPredictveText'],
                expected_activeview="freetext"
            )
        ),
    ]

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_string("active-language", "zh-hant")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def test_chewing(self):
        """Switching to traditional Chinese should result in chewing characters
        being entered via autocomplete regardless of layout or prediction
        being disabled.

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('hk4\n')

        expected = "冊\n"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_bopomofo(self):
        """Individual characters should enter phonetic input (bopomofo)

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('a\n')

        expected = "ㄇ\n"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_auto_punctuation(self):
        """A chinese full-stop character should be entered after space has
        been pressed twice.

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('hk4  ')

        expected = "冊。 "

        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_phrases(self):
        """It should be possible to type an entire phrase with chewing,
        and only commit at the end of the phrase.

        """
        text_area = self.launch_test_input_area(self.label, self.hints)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('2j6gj cjo4y94w961o3\n')

        expected = "讀書會在台北\n"

        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardSelection(LomiriKeyboardTests):

    def test_delete_selection(self):
        """Selecting a word and then pressing backspace should delete the
        world.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Testing the selection deletion')

        # Double tap to select a word
        self.pointer.click_object(text_area)
        self.pointer.click_object(text_area)

        keyboard.type('\b')

        expected = 'Testing the  deletion'
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_selection_focus(self):
        """Focusing on a field with selected text should leave the text
        unchanged.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('This is a test')

        # Double tap to select a word
        self.pointer.click_object(text_area)
        self.pointer.click_object(text_area)

        keyboard.dismiss()

        self.ensure_focus_on_input(text_area)

        expected = 'This is a test'
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardEmoji(LomiriKeyboardTests):

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_strv("enabled-languages", ["en", "emoji"])
        gsettings.set_string("previous-language", "emoji")
        gsettings.set_string("active-language", "emoji")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def test_emoji_input(self):
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard = Keyboard()

        sleep(2)

        keyboard.type('😁😆😃😏')

        expected = "😁😆😃😏"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_emoji_deletion(self):
        """Emoji characters should be deleted completely, despite being made up
           of multiple bytes.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard = Keyboard()

        sleep(2)

        keyboard.type('😁😆😃😏\b')

        expected = "😁😆😃"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardLanguageMenu(LomiriKeyboardTests):

    def setUp(self):
        super().setUp()
        self.gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")

    def _set_keyboard_language(self, language):
        self.gsettings.set_string("active-language", language)

    def _get_keyboard_language(self):
        return self.gsettings.get_string("active-language")

    def _get_plugin_path(self, language):
        path = ("file:///usr/share/maliit/plugins/com/ubuntu/lib/{lang}/"
                "Keyboard_{lang}.qml")
        return path.format(lang=language)

    def test_tapping(self):
        """Tapping the language menu key should switch to the previously
        used language.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        # Make sure the previous language is es and the current language is en
        self._set_keyboard_language("es")
        self.assertThat(
            keyboard._plugin_source,
            Eventually(Equals(self._get_plugin_path("es"))))

        self._set_keyboard_language("en")
        self.assertThat(
            keyboard._plugin_source,
            Eventually(Equals(self._get_plugin_path("en"))))

        keyboard.press_key("language")

        self.assertThat(
            keyboard._plugin_source,
            Eventually(Equals(self._get_plugin_path("es"))))

    def test_long_press(self):
        """Holding down the language menu key should switch display the
        language switcher menu.

        """

        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        self.assertThat(self._get_keyboard_language(), Equals("en"))

        keyboard.press_key("language", long_press=True)

        menu = keyboard.maliit.select_single("LanguageMenu")

        self.assertThat(
            menu.visible,
            Eventually(Equals(True))
        )

        keyboard.press_key("language")

    def test_switching_with_preedit(self):
        """Switching languages whilst text is in preedit should result in
        that text being committed.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Hello')

        keyboard.press_key("language")

        expected = 'Hello'
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardOxide(LomiriKeyboardTests):

    def test_autocomplete(self):
        """Test that words are auto-completed when entered into an oxide text
        field.

        """
        qml = dedent("""
        import QtQuick 2.4
        import Lomiri.Components 1.3
        import Lomiri.Web 0.2

        Rectangle {
            id: window
            objectName: "windowRectangle"
            color: "lightgrey"

            WebView {
                anchors.fill: parent
                objectName: "webview"
                Component.onCompleted: {
                    loadHtml("
                        <html><body><textarea id='textarea'
                        onkeyup=\\\"document.title=
                        document.getElementById('textarea').value;\\\"
                        style='width: 100%; height: 100%;'>
                        </textarea></body></html>"
                    );
                }
            }
        }

        """)
        app = self._start_qml_script(qml)
        webview = app.select_single(objectName='webview')

        self.ensure_focus_on_input(webview)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Pic ')

        expected = 'Picture'
        self.assertThat(
            webview.title,
            Eventually(Equals(expected))
        )

    def test_double_space(self):
        """Test that double space still inserts a full-stop and replaces all
        white spaces characters, even if they're non breaking spaces.

        """
        qml = dedent("""
        import QtQuick 2.4
        import Lomiri.Components 1.3
        import Lomiri.Web 0.2

        Rectangle {
            id: window
            objectName: "windowRectangle"
            color: "lightgrey"

            WebView {
                anchors.fill: parent
                objectName: "webview"
                Component.onCompleted: {
                    loadHtml("
                        <html><body><textarea id='textarea'
                        onkeyup=\\\"document.title=
                        document.getElementById('textarea').value;\\\"
                        style='width: 100%; height: 100%;'>&nbsp;</textarea>
                        </body></html>"
                    );
                }
            }
        }

        """)
        app = self._start_qml_script(qml)
        webview = app.select_single(objectName='webview')

        self.ensure_focus_on_input(webview)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('  ')

        # The page title trims white space, so we just look for the
        # full-stop, rather than '. '
        expected = "."
        self.assertThat(
            webview.title,
            Eventually(Equals(expected))
        )

    def test_hiding(self):
        """Verify that the keyboard remains hidden after being dismissed from
        a field that is no longer enabled.

        """
        qml = dedent("""
        import QtQuick 2.4
        import Lomiri.Components 1.1
        import Lomiri.Web 0.2

        Rectangle {
            id: window
            objectName: "windowRectangle"
            color: "lightgrey"

            WebView {
                anchors.fill: parent
                objectName: "webview"
                Component.onCompleted: {
                    loadHtml("
                        <html><body><div id='scroll' style='width: 100%;
                        height: 200%; position: absolute; background: green;
                        visibility: hidden;'></div><input id='input'
                        style='height: 50%; width: 100%' type='text'
                        onkeyup=\\\"if (event.keyCode == 13)
                        {document.getElementById('input').disabled=true;
                        document.getElementById('scroll').style.visibility=
                        'visible';}\\\" style='width: 100%%;
                        height: 100%%;' /></body></html>");
                }
            }
        }

        """)
        app = self._start_qml_script(qml)
        webview = app.select_single(objectName='webview')

        self.ensure_focus_on_input(webview)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Test\n')

        keyboard.dismiss()

        pointer = Pointer(Touch.create())
        pointer.drag(
            webview.width / 2.0,
            webview.height / 2.0,
            webview.width / 2.0,
            webview.height / 2.0 + 100
        )

        self.assertThat(
            keyboard.is_available,
            Eventually(Equals(False))
        )

    def test_double_caps(self):
        """Ensure that we switch back to lowercase after typing a letter in
        Oxide.

        """
        qml = dedent("""
        import QtQuick 2.4
        import Lomiri.Components 1.3
        import Lomiri.Web 0.2

        Rectangle {
            id: window
            objectName: "windowRectangle"
            color: "lightgrey"

            WebView {
                anchors.fill: parent
                objectName: "webview"
                Component.onCompleted: {
                    loadHtml("
                        <html><body><textarea id='textarea'
                        style='width: 100%; height: 100%;'></textarea>
                        </body></html>"
                    );
                }
            }
        }

        """)
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", False)
        gsettings.set_boolean("predictive-text", False)
        gsettings.set_boolean("spell-checking", False)

        app = self._start_qml_script(qml)
        webview = app.select_single(objectName='webview')

        self.ensure_focus_on_input(webview)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('H')

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.NORMAL))
        )


class LomiriKeyboardPluginPaths(LomiriKeyboardTests):

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_strv(
            "plugin-paths", [
                "/custom/share/maliit/plugins/com/ubuntu/lib",
                "/usr/share/maliit/tests/lomiri-keyboard/"
            ])
        gsettings.set_strv("enabled-languages", ["en", "testlayout"])
        gsettings.set_string("previous-language", "testlayout")
        gsettings.set_string("active-language", "testlayout")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def test_typing(self):
        """Test that typing works using a plugin loaded from a custom location.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        # The test layout has a single key that enters the word 'Test'
        keyboard.press_key("Test")

        expected = "Test"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardLayouts(LomiriKeyboardTests):

    scenarios = []

    @classmethod
    def setUpClass(cls):
        # Layout, TLD, Text
        test_data = [
            ("ar", ".eg", "ازتبار"),
            ("az", ".com", "çheck"),
            ("bs", ".com", "test"),
            ("ca", ".com", "çheck"),
            ("cs", ".cz", "test"),
            ("da", ".dk", "tæst"),
            ("de", ".de", "triäl"),
            ("el", ".gr", "δοκιμη"),
            ("en", ".com", "test"),
            ("eo", ".edu", "ŝĝŭĵĉ"),
            ("es", ".es", "testiñg"),
            ("fi", ".fi", "triäl"),
            ("fr", ".fr", "çheck"),
            ("gd", ".co.uk", "test"),
            ("he", ".il", "מבחן"),
            ("hr", ".com", "test"),
            ("hu", ".hu", "test"),
            ("is", ".is", "tæst"),
            ("it", ".it", "test"),
            ("lv", ".lv", "test"),
            ("nb", ".no", "bokmål"),
            ("nl", ".nl", "test"),
            ("pl", ".pl", "tęst"),
            ("pt", ".com.br", "çheck"),
            ("ro", ".com", "test"),
            ("ru", ".ru", "тест"),
            ("sl", ".com", "test"),
            ("sr", ".срб", "тест"),
            ("sv", ".se", "triäl"),
            ("uk", ".укр", "тест")
        ]

        for entry in test_data:
            cls.scenarios.append((
                "%s_free" % entry[0],
                dict(
                    layout=entry[0],
                    hints=['Qt.ImhNoPredictiveText'],
                    expected_activeview="freetext",
                    tld=entry[1],
                    text=entry[2]
                )
            ))
            cls.scenarios.append((
                "%s_url" % entry[0],
                dict(
                    layout=entry[0],
                    hints=['Qt.ImhUrlCharactersOnly'],
                    expected_activeview="url",
                    tld=entry[1],
                    text=entry[2] + entry[1]
                )
            ))
            cls.scenarios.append((
                "%s_email" % entry[0],
                dict(
                    layout=entry[0],
                    hints=['Qt.ImhEmailCharactersOnly'],
                    expected_activeview="email",
                    tld=entry[1],
                    text=entry[2] + "@" + entry[2] + entry[1]
                )
            ))

    def test_layouts(self):
        """Test all layout plugins in freetext, url and email mode.

        """
        text_area = self.launch_test_input_area(
            self.layout + " - " + self.expected_activeview,
            self.hints
        )
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_string("active-language", self.layout)

        self.assertThat(
            gsettings.get_string("active-language"),
            Equals(self.layout)
        )

        sleep(2)

        if self.text[-len(self.tld):] == self.tld:
            keyboard.type(self.text[:-len(self.tld)])
            keyboard.press_key(self.tld)
        else:
            keyboard.type(self.text)

        self.assertThat(
            text_area.text,
            Eventually(Equals(self.text))
        )


class LomiriKeyboardCursorTests(LomiriKeyboardTests):

    def test_cursor_movement(self):
        """Test that autopilot is able to move the cursor

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type("Cursor Test")

        keyboard.send_home_key()
        keyboard.send_right_key()
        keyboard.send_right_key()
        keyboard.type("\b")
        keyboard.send_end_key()
        keyboard.send_left_key()
        keyboard.type("\b")

        expected = "Crsor Tet"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardJapaneseTests(LomiriKeyboardTests):

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_string("active-language", "ja")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def test_japanese_input(self):
        """Test top level keys on Japanese layout.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.pointer.click_object(text_area)
        keyboard = Keyboard()
        self.assertThat(keyboard.is_available, Eventually(Equals(True)))

        text = "あかさたなはまやら"
        keyboard.type(text)
        keyboard.press_key('\n')

        self.assertThat(
            text_area.text,
            Eventually(Equals(text))
        )

    def test_japanese_flick(self):
        """Test pop-up flick input characters.

        """
        # centre: あ, up: う, down: お, left: い, right: え
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.pointer.click_object(text_area)
        keyboard = Keyboard()
        self.assertThat(keyboard.is_available, Eventually(Equals(True)))

        gu = float(os.environ.get('GRID_UNIT_PX', 8))
        keyboard.press_key('あ', slide_offset=4 * gu)
        keyboard.press_key('あ', slide_offset=-4 * gu)
        keyboard.press_key('\n')

        expected = "えい"

        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class LomiriKeyboardKoreanTests(LomiriKeyboardTests):

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.lomiri.keyboard.maliit")
        gsettings.set_string("active-language", "ko")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def test_korean_input(self):
        """Test keys on Korean layout.

        """
        text_area = self.launch_test_input_area(
            input_hints=['Qt.ImhNoPredictiveText'])
        self.pointer.click_object(text_area)
        keyboard = Keyboard()
        self.assertThat(keyboard.is_available, Eventually(Equals(True)))

        expected = "한글 "
        keyboard.press_key('ㅎ')
        keyboard.press_key('ㅏ')
        keyboard.press_key('ㄴ')
        keyboard.press_key('ㄱ')
        keyboard.press_key('ㅡ')
        keyboard.press_key('ㄹ')
        keyboard.press_key(' ')

        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


def maliit_cleanup():
    maliitdir = os.path.expanduser("~/.local/share/maliit-server")
    if os.path.exists(maliitdir + ".bak") and os.path.exists(maliitdir):
        shutil.rmtree(maliitdir)
        os.rename(maliitdir + ".bak", maliitdir)
    subprocess.check_call(['restart', 'maliit-server'])


# Clear away any learnt predictions and recent emoji
maliitdir = os.path.expanduser("~/.local/share/maliit-server")
if os.path.exists(maliitdir):
    os.rename(maliitdir, maliitdir + ".bak")
subprocess.check_call(['initctl', 'set-env', 'QT_LOAD_TESTABILITY=1'])
subprocess.check_call(['restart', 'maliit-server'])

atexit.register(maliit_cleanup)

# FIXME: Workaround re: lp:1248902 and lp:1248913
logger.debug("Waiting for maliit-server to be ready")
sleep(10)
