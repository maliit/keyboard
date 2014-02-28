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

import os
import subprocess

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
from unity8 import process_helpers
from unity8.shell.emulators.dash import Dash
from unity8.shell.emulators import UnityEmulatorBase
from ubuntuuitoolkit import base

from ubuntu_keyboard.emulators.keyboard import Keyboard
from ubuntu_keyboard.emulators.keypad import KeyPadState

import logging


logger = logging.getLogger(__name__)


def _stop_unity8():
    status = process_helpers._get_unity_status()
    if "start/" in status:
        try:
            logger.debug("Stopping unity8")
            subprocess.check_call(['initctl', 'stop', 'unity8'])
        except subprocess.CalledProcessError as e:
            e.args += ("Unable to stop unity8",)
            raise
    else:
        logger.debug("No need to stop unity.")


def _start_unity8():
    status = process_helpers._get_unity_status()
    if "stop/" in status:
        try:
            logger.debug("Starting unity8")
            subprocess.check_call(['initctl', 'start', 'unity8'])
        except subprocess.CalledProcessError as e:
            e.args += ("Unable to start unity8",)
            raise
    else:
        raise RuntimeError(
            "Unable to start unity8: server is currently running."
        )


def _assertUnityReady():
        unity_pid = process_helpers._get_unity_pid()
        unity = get_proxy_object_for_existing_process(
            pid=unity_pid,
            emulator_base=UnityEmulatorBase,
        )
        dash = unity.wait_select_single(Dash)
        home_scope = dash.get_scope('home')

        home_scope.isLoaded.wait_for(True)
        home_scope.isCurrent.wait_for(True)


def _restart_unity8():
    _stop_unity8()
    _start_unity8()


class UbuntuKeyboardTests(AutopilotTestCase):
    maliit_override_file = os.path.expanduser(
        "~/.config/upstart/maliit-server.override"
    )

    @classmethod
    def setUpClass(cls):
        try:
            logger.debug("Creating the override file.")
            with open(
                UbuntuKeyboardTests.maliit_override_file, 'w'
            ) as override_file:
                override_file.write("exec maliit-server -testability")

            process_helpers.restart_unity_with_testability()
            _assertUnityReady()
            #### FIXME: This is a work around re: lp:1238417 ####
            if model() != "Desktop":
                from autopilot.input import _uinput
                _uinput._touch_device = _uinput.create_touch_device()
            ####

            #### FIXME: Workaround re: lp:1248902 and lp:1248913
            logger.debug("Waiting for maliit-server to be ready")
            sleep(10)
            ####

        except IOError as e:
            e.args += (
                "Failed attempting to write override file to {file}".format(
                    file=UbuntuKeyboardTests.maliit_override_file
                ),
            )
            raise

    @classmethod
    def tearDownClass(cls):
        try:
            os.remove(UbuntuKeyboardTests.maliit_override_file)
        except OSError:
            logger.warning("Attempted to remove non-existent override file")
        _restart_unity8()

    def setUp(self):
        if model() == "Desktop":
            self.skipTest("Ubuntu Keyboard tests only run on device.")
        super(UbuntuKeyboardTests, self).setUp()
        self.pointer = Pointer(Touch.create())

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

        desktop_file = self._write_test_desktop_file()
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
        if input_hints is None:
            extra_script = "Qt.ImhNoPredictiveText"
        else:
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
                anchors {
                    top: inputLabel.bottom
                    horizontalCenter: parent.horizontalCenter
                    topMargin: 10
                }
                inputMethodHints: %(input_method)s
            }
        }

        """ % {'label': label, 'input_method': extra_script})

        return self._start_qml_script(simple_script)


class UbuntuKeyboardTestsAccess(UbuntuKeyboardTests):

    def test_keyboard_is_available(self):
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)
        app = self._launch_simple_input()
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
        text_area = self.launch_test_input_area(label=self.label)
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(self.input)
        self.assertThat(text_area.text, Eventually(Equals(self.input)))


class UbuntuKeyboardStateChanges(UbuntuKeyboardTests):

    # Note: this is a failing test due to bug lp:1214695
    # Note: based on UX design doc
    def test_keyboard_layout_starts_shifted(self):
        """When first launched the keyboard state must be
        shifted/capitalised.

        """
        self.skip(
            "Skipping as feature hasn't landed yet, refer to bug lp:1214695"
        )

        text_area = self.launch_test_input_area()
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
        self.skip(
            "Skipping due to bug in emulator: lp:1237846"
        )

        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type('abc')
        # Bug lp:1229003 and lp:1229001
        sleep(.2)
        keyboard.press_key('shift')
        keyboard.press_key('shift')

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.CAPSLOCK))
        )

    # Note: based on UX design doc
    def test_shift_state_returns_to_default_after_letter_typed(self):
        """Pushing shift and then typing an uppercase letter must automatically
        shift the keyboard back into the default state.

        """
        text_area = self.launch_test_input_area()
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

    # Note: this is a failing test due to bug lp:1214695
    # Note: Based on UX design doc.
    def test_shift_state_entered_after_fullstop(self):
        """After typing a fullstop the keyboard state must automatically
        enter the shifted state.

        """
        self.skip(
            "Skipping as feature hasn't landed yet, refer to bug lp:1214695"
        )
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type("abc.")

        self.assertThat(
            text_area.text,
            Eventually(Equals("abc."))
        )

        self.assertThat(
            keyboard.active_keypad_state,
            Eventually(Equals(KeyPadState.SHIFTED))
        )

    def test_switching_between_states(self):
        """The user must be able to type many different characters including
        spaces and backspaces.

        """
        text_area = self.launch_test_input_area()
        self.ensure_focus_on_input(text_area)
        keyboard = Keyboard()
        self.addCleanup(keyboard.dismiss)

        keyboard.type(
            'abc gone\b\b &  \bABC (123)'
        )

        expected = "abc go & ABC (123)"
        self.assertThat(
            text_area.text,
            Eventually(Equals(expected))
        )


class UbuntuKeyboardInputTypeStateChange(UbuntuKeyboardTests):
    """Note: these tests are currently failing due to bug lp:1214694 (the
    activeView detail isn't exposed correctly nor is it updated as expected
    (i.e. when the view changes.))

    """

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
            "Number",
            dict(
                label="Number",
                hints=['Qt.ImhFormattedNumbersOnly'],
                expected_activeview="number"
            )
        ),
        (
            "Telephone",
            dict(
                label="Telephone",
                hints=['Qt.ImhDigitsOnly'],
                expected_activeview="number"
            )
        ),
    ]

    # Note: based on UX design doc
    @skip("Unable to determine LayoutId re: bug lp:1248796")
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
