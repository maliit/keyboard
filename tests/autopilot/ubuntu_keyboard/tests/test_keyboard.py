# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
#
# Ubuntu Keyboard Test Suite
# Copyright (C) 2013 Canonical
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

import os, os.path
import shutil
import subprocess
import atexit

from testtools import skip
from testtools.matchers import Equals
import tempfile
from textwrap import dedent
from time import sleep

from autopilot.testcase import AutopilotTestCase
from autopilot.input import Pointer, Touch
from autopilot.introspection import get_proxy_object_for_existing_process
from autopilot.matchers import Eventually
from autopilot.platform import model
from ubuntuuitoolkit import base

from ubuntu_keyboard.emulators.keyboard import Keyboard
from ubuntu_keyboard.emulators.keypad import KeyPadState

from gi.repository import Gio

import logging


logger = logging.getLogger(__name__)


class UbuntuKeyboardTests(AutopilotTestCase):

    @classmethod
    def setUpClass(cls):
        #### FIXME: This is a work around re: lp:1238417 ####
        if model() != "Desktop":
            from autopilot.input import _uinput
            _uinput._touch_device = _uinput.create_touch_device()
        ####

    def setUp(self):
        if model() == "Desktop":
            self.skipTest("Ubuntu Keyboard tests only run on device.")
        super(UbuntuKeyboardTests, self).setUp()
        self.set_test_settings()
        sleep(5) # Have to give time for gsettings change to propogate
        self.pointer = Pointer(Touch.create())

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.canonical.keyboard.maliit")
        gsettings.set_strv("enabled-languages", ["en", "es", "de", "zh", "emoji"])
        gsettings.set_string("active-language", "en")
        gsettings.set_string("previous-language", "es")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def launch_test_input_area(self, label="", input_hints=None):
        self.app = self._launch_simple_input(label, input_hints)
        text_area = self.app.select_single("QQuickTextInput")

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
        desktop_file = "/usr/share/applications/ubuntu-keyboard-tester.desktop"
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
        import QtQuick 2.0
        import Ubuntu.Components 0.1

        Rectangle {
            id: window
            objectName: "windowRectangle"
            color: "lightgrey"

            Text {
                id: inputLabel
                text: "%(label)s"
                font.pixelSize: units.gu(3)
                anchors {
                    left: input.left
                    top: parent.top
                    topMargin: 25
                    bottomMargin: 25
                }
            }

            TextField {
                id: input;
                objectName: "input"
                anchors.centerIn: parent
                inputMethodHints: %(input_method)s
            }
        }

        """ % {'label': label, 'input_method': extra_script})

        return self._start_qml_script(simple_script)


class UbuntuKeyboardTestsAccess(UbuntuKeyboardTests):

    def test_keyboard_is_available(self):
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)
        app = self._launch_simple_input(input_hints=['Qt.ImhNoPredictiveText'])
        text_rectangle = app.select_single("QQuickTextInput")

        self.pointer.click_object(text_rectangle)

        self.assertThat(keyboard.is_available, Eventually(Equals(True)))


class UbuntuKeyboardTypingTests(UbuntuKeyboardTests):

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
        text_area = self.launch_test_input_area(label=self.label, input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(self.input)
        self.assertThat(text_area.text, Eventually(Equals(self.input)))


class UbuntuKeyboardStateChanges(UbuntuKeyboardTests):

    def test_keyboard_layout_starts_shifted(self):
        """When first launched the keyboard state must be
        shifted/capitalised.

        """
        text_area = self.launch_test_input_area(input_hints=['Qt.ImhNoPredictiveText'])
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
        text_area = self.launch_test_input_area(input_hints=['Qt.ImhNoPredictiveText'])
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('abc')
        # Bug lp:1229003 and lp:1229001
        sleep(.2)
        keyboard.press_key('shift')
        keyboard.press_key('shift', True)

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.CAPSLOCK))
        )

    # Note: based on UX design doc
    def test_shift_state_returns_to_default_after_letter_typed(self):
        """Pushing shift and then typing an uppercase letter must automatically
        shift the keyboard back into the default state.

        """
        text_area = self.launch_test_input_area(input_hints=['Qt.ImhNoPredictiveText'])
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
        text_area = self.launch_test_input_area(input_hints=['Qt.ImhNoPredictiveText'])
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
        text_area = self.launch_test_input_area(input_hints=['Qt.ImhNoPredictiveText'])
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
        text_area = self.launch_test_input_area(input_hints=['Qt.ImhNoPredictiveText'])
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
        import QtQuick 2.0
        import Ubuntu.Components 0.1

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


class UbuntuKeyboardInputTypeStateChange(UbuntuKeyboardTests):

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


class UbuntuKeyboardAdvancedFeatures(UbuntuKeyboardTests):

    def test_double_space_fullstop(self):
        """After tapping space twice a fullstop should be entered.

        """
        text_area = self.launch_test_input_area(input_hints=['Qt.ImhNoPredictiveText'])
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
        """After typing 'i' followed by a space it should get auto-corrected to 'I'
        via the override mechanism.

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

        keyboard.type('Helfn ')

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

        keyboard.type("'here' 'to' ")

        expected = "'here' 'to' "
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class UbuntuKeyboardPinyin(UbuntuKeyboardTests):

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
        gsettings = Gio.Settings.new("com.canonical.keyboard.maliit")
        gsettings.set_string("active-language", "zh")
        gsettings.set_boolean("auto-capitalization", True)
        gsettings.set_boolean("auto-completion", True)
        gsettings.set_boolean("predictive-text", True)
        gsettings.set_boolean("spell-checking", True)
        gsettings.set_boolean("double-space-full-stop", True)

    def test_pinyin(self):
        """Switching to Chinese should result in pinyin characters being entered
           via autocomplete regardless of layout or prediction being disabled.

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


class UbuntuKeyboardSelection(UbuntuKeyboardTests):

    def test_delete_selection(self):
        """Selecting a word and then pressing backspace should delete the world.
        
        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('Testing selection deletion')

        # Double tap to select a word
        self.pointer.click_object(text_area)
        self.pointer.click_object(text_area)

        keyboard.type('\b')

        expected = 'Testing  deletion'
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )

    def test_selection_focus(self):
        """Focusing on a field with selected text should leave the text unchanged.

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


class UbuntuKeyboardEmoji(UbuntuKeyboardTests):

    def set_test_settings(self):
        gsettings = Gio.Settings.new("com.canonical.keyboard.maliit")
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

        keyboard.press_key("language")

        sleep(5)

        keyboard = Keyboard()

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

        keyboard.press_key("language")

        sleep(5)

        keyboard = Keyboard()

        keyboard.type('😁😆😃😏\b')

        expected = "😁😆😃"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class UbuntuKeyboardLanguageMenu(UbuntuKeyboardTests):

    def test_tapping(self):
        """Tapping the language menu key should switch to the previously
        used language.

        """

        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        gsettings = Gio.Settings.new("com.canonical.keyboard.maliit")
        self.assertThat(
            gsettings.get_string("active-language"),
            Equals('en')
        )

        keyboard.press_key("language")

        sleep(5)

        self.assertThat(
            gsettings.get_string("active-language"),
            Equals('es')
        )

    def test_long_press(self):
        """Holding down the language menu key should switch display the
        language switcher menu.

        """

        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        gsettings = Gio.Settings.new("com.canonical.keyboard.maliit")
        self.assertThat(
            gsettings.get_string("active-language"),
            Equals('en')
        )

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
        

def maliit_cleanup():
    presagedir = os.path.expanduser("~/.presage")
    if os.path.exists(presagedir + ".bak") and os.path.exists(presagedir):
        shutil.rmtree(presagedir)
        os.rename(presagedir + ".bak", presagedir)
    subprocess.check_call(['restart', 'maliit-server'])

# Clear away any learnt predictions
presagedir = os.path.expanduser("~/.presage")
if os.path.exists(presagedir):
    os.rename(presagedir, presagedir + ".bak")
subprocess.check_call(['initctl', 'set-env', 'QT_LOAD_TESTABILITY=1'])
subprocess.check_call(['restart', 'maliit-server'])

atexit.register(maliit_cleanup)

#### FIXME: Workaround re: lp:1248902 and lp:1248913
logger.debug("Waiting for maliit-server to be ready")
sleep(10)
####
