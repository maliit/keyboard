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

from collections import defaultdict

from ubuntu_keyboard.emulators import UbuntuKeyboardEmulatorBase
from ubuntu_keyboard.emulators.keypad import KeyPad

from time import sleep
import logging

from autopilot.input import Pointer, Touch
from autopilot.introspection import (
    get_proxy_object_for_existing_process,
    ProcessSearchError
)
from autopilot.introspection.dbus import StateNotFoundError


logger = logging.getLogger(__name__)


class KeyboardState:
    character = "CHARACTERS"
    symbol = "SYMBOLS"


class Keyboard(object):
    """Emulator that provides the OSK as an input backend."""

    _action_to_label = {
        'SHIFT': 'shift',
        '\b': 'backspace',
        'ABC': 'symbols',
        '?123': 'symbols',
        ' ': 'space',
        '\n': 'return',
    }

    __maliit = None

    def __init__(self, pointer=None):
        try:
            self.orientation = self.maliit.select_single(
                "OrientationHelper"
            )
        except ValueError as e:
            e.args += (
                "More than one OrientationHelper object was found, aborting.",
            )
            raise
        except StateNotFoundError:
            logger.error("Unable to find the Orientation Helper, aborting.")
            raise

        try:
            self.keyboard = self.maliit.select_single(
                "QQuickItem",
                objectName="ubuntuKeyboard"
            )
        except ValueError as e:
            e.args += (
                "There was more than one Keyboard object found, aborting.",
            )
            raise
        except StateNotFoundError:
            logger.error(
                "Unable to find the Ubuntu Keyboard object within the "
                "maliit server, aborting."
            )
            raise

        self._keyboard_container = self.keyboard.select_single(
            "KeyboardContainer"
        )

        self._stored_active_keypad_name = None
        self._active_keypad = None

        self._keys_position = defaultdict(dict)
        self._keys_contained = defaultdict(dict)

        self._store_current_orientation()

        if pointer is None:
            self.pointer = Pointer(Touch.create())
        else:
            self.pointer = pointer

    @property
    def maliit(self):
        # We cache __mallit_server as a class attribute because
        # get_proxy_object_for_existing_process clears backends for proxy
        # objects, this means that if this is called twice within a test the
        # first keyboard object created has now lost all its _Backends.
        if Keyboard.__maliit is None:
            try:
                Keyboard.__maliit = get_proxy_object_for_existing_process(
                    connection_name='org.maliit.server',
                    emulator_base=UbuntuKeyboardEmulatorBase
                )

                if Keyboard.__maliit is None:
                    raise RuntimeError("Maliit Server could not be found.")
            except ProcessSearchError as e:
                e.args += (
                    "Unable to find maliit-server dbus object. Has it been "
                    "started with introspection enabled?",
                )
                raise
        return Keyboard.__maliit

    def dismiss(self):
        """Swipe the keyboard down to hide it.

        :raises: *AssertionError* if the state.wait_for fails meaning that the
         keyboard failed to hide.

        """
        if self.is_available():
            x, y, h, w = self._keyboard_container.globalRect
            x_pos = int(w / 2)
            # start_y: just inside the keyboard, must be a better way than +1px
            start_y = y - 10
            end_y = y + int(h / 2)
            self.pointer.drag(x_pos, start_y, x_pos, end_y)

            self.keyboard.state.wait_for("HIDDEN")

    def is_available(self):
        """Returns true if the keyboard is shown and ready to use."""
        return (self.keyboard.state == "SHOWN")

    # Much like is_available, but attempts to wait for the keyboard to be
    # ready.
    def wait_for_keyboard_ready(self, timeout=10):
        """Waits for *timeout* for the keyboard to be ready and returns
        true. Returns False if the keyboard fails to be considered ready within
        the alloted time.

        """
        try:
            self.keyboard.state.wait_for("SHOWN", timeout=timeout)
            return True
        except AssertionError:
            return False

    def press_key(self, key):
        """Tap on the key with the internal pointer

        :params key: String containing the text of the key to tap.

        :raises: *RuntimeError* if the keyboard is not available and thus not
          ready to be used.
        :raises: *ValueError* if the supplied key cannot be found on any of
          the the current keyboards layouts.
        """
        if not self.is_available():
            raise RuntimeError("Keyboard is not on screen")

        key = self._translate_key(key)

        req_keypad = KeyboardState.character
        req_key_state = self._keypad_contains_key(req_keypad, key)
        if req_key_state is None:
            req_keypad = KeyboardState.symbol
            req_key_state = self._keypad_contains_key(req_keypad, key)

        if req_key_state is None:
            raise ValueError("Key '%s' was not found on the keyboard" % key)

        key_pos = self._get_key_pos_from_keypad(req_keypad, key)
        self._show_keypad(req_keypad)
        self._change_keypad_to_state(req_key_state)

        self._tap_key(key_pos)

    def type(self, string, delay=0.1):
        """Type the string *string* with a delay of *delay* between each key
        press

        .. note:: The delay provides a minimum delay, it may take longer
        between each press as the keyboard shifts between states etc.

        Only 'normal' or single characters can be typed this way.

        :raises: *ValueError* if one of the the supplied keys cannot be
          found on any of the the current keyboards layouts.

        """
        for char in string:
            self.press_key(char)
            sleep(delay)

    @property
    def current_state(self):
        return self.keyboard.state

    @property
    def active_keypad_state(self):
        return self._keyboard_container.activeKeypadState

    @property
    def active_keypad(self):
        need_to_update = False
        if self._active_keypad is None:
            need_to_update = True
        else:
            try:
                # Check if the current keypad object still exists.
                self._active_keypad.enabled
            except StateNotFoundError:
                need_to_update = True

        if (
            need_to_update
            or self._stored_active_keypad_name != self._current_keypad_name
            or self._keyboard_details_changed()
        ):
            self._stored_active_keypad_name = self._current_keypad_name
            loader = self.maliit.select_single(
                "QQuickLoader",
                objectName='characterKeyPadLoader'
            )
            logger.debug("Keypad lookup")
            self._active_keypad = loader.select_single(KeyPad)
        return self._active_keypad

    @property
    def _current_keypad_name(self):
        return self._keyboard_container.state

    def _update_details_for_keypad(self, keypad_name):
        self._show_keypad(keypad_name)

        contained, positions = self.active_keypad.get_key_details()
        self._keys_contained[self._keyboard_container.state] = contained
        self._keys_position[self._keyboard_container.state] = positions

    def _keypad_contains_key(self, keypad_name, key):
        """Returns the keypad state that key is found in.

        Returns either a KeyPadState if the key is found on the provided keypad
        or None if not found.

        """
        if self._keypad_details_expired(keypad_name):
            self._update_details_for_keypad(keypad_name)

        return self._keys_contained[keypad_name].get(key, None)

    def _get_key_pos_from_keypad(self, keypad_name, key):
        """Returns the position of the key if it is found on that keypad or
        None if it is not.

        """
        if self._keypad_details_expired(keypad_name):
            self._update_details_for_keypad(keypad_name)

        return self._keys_position[keypad_name].get(key, None)

    def _show_keypad(self, keypad_name):
        if self._current_keypad_name == keypad_name:
            return

        key_pos = self._get_key_pos_from_keypad(
            self._current_keypad_name,
            "symbols"
        )
        self._tap_key(key_pos)
        self._current_keypad_name.wait_for(keypad_name)
        self.active_keypad.opacity.wait_for(1.0)

    def _change_keypad_to_state(self, state):
        if self._keyboard_container.activeKeypadState == state:
            return

        key_pos = self._get_key_pos_from_keypad(
            self._current_keypad_name,
            "shift"
        )
        self._tap_key(key_pos)
        self._keyboard_container.activeKeypadState.wait_for(state)
        self.active_keypad.opacity.wait_for(1.0)

    def _tap_key(self, key_rect, pointer=None):
        if pointer is None:
            pointer = Pointer(Touch.create())
        pointer.click_object(key_rect)

    def _keyboard_details_changed(self):
        return self._orientation_changed()

    def _keypad_details_expired(self, keypad_name):
        return (
            self._keys_contained.get(keypad_name) is None
            or self._keyboard_details_changed()
        )

    def _orientation_changed(self):
        if self._stored_orientation != self.orientation.orientationAngle:
            self._store_current_orientation()
            return True
        else:
            return False

    def _store_current_orientation(self):
        self._stored_orientation = self.orientation.orientationAngle

    def _translate_key(self, label):
        """Get the label for a 'special key' (i.e. space) so that it can be
        addressed and clicked.

        """
        return Keyboard._action_to_label.get(label, label)
