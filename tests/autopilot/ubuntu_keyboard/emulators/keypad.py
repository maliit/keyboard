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

from ubuntu_keyboard.emulators import UbuntuKeyboardEmulatorBase
from ubuntu_keyboard.emulators.key import Key

import logging

from autopilot.input import Pointer, Touch

logger = logging.getLogger(__name__)


class KeyPad(UbuntuKeyboardEmulatorBase):
    """An emulator that understands the KeyPad and its internals and how to
    interact with it.

      - Which keys are displayed within it
      - The positions of these keys
      - The state (NORMAL/SHIFTED) the KeyPad is in

    """

    class State:
        NORMAL = "NORMAL"
        SHIFTED = "SHIFTED"

    def __init__(self, *args):
        super(KeyPad, self).__init__(*args)
        self._key_pos = dict()
        self._contained_keys = []
        self._contained_shifted_keys = []

        self.update_key_positions()
        self.update_contained_keys()

    def contains_key(self, label):
        """Returns true if a key with the label *label* is contained within
        this KeyPad.

        """
        return (label in self._contained_keys
                or label in self._contained_shifted_keys)

    def update_key_positions(self):
        """Store the positions of the keys that are contained within this
        KeyPad.

        """
        def _iter_keys(key_type, label_fn):
            for key in self.select_many(key_type):
                with key.no_automatic_refreshing():
                    key_pos = Key.Pos(*key.globalRect)
                    label = label_fn(key)
                    if label != '':
                        self._key_pos[label] = key_pos
                    if key.shifted != '':
                        self._key_pos[key.shifted] = key_pos

        _iter_keys("CharKey", lambda x: x.label)
        _iter_keys("ActionKey", lambda x: x.action)

    def update_contained_keys(self):
        """Store which keys are contained within this KeyPad."""
        def _iter_keys(key_type, label_fn):
            for key in self.select_many(key_type):
                with key.no_automatic_refreshing():
                    label = label_fn(key)
                    if label != '':
                        self._contained_keys.append(label)
                    if key.shifted != '':
                        self._contained_shifted_keys.append(key.shifted)

        _iter_keys("CharKey", lambda x: x.label)
        _iter_keys("ActionKey", lambda x: x.action)

    def _get_keys_required_keypad_state(self, key):
        if key in self._contained_keys:
            return KeyPad.State.NORMAL
        elif key in self._contained_shifted_keys:
            return KeyPad.State.SHIFTED
        else:
            raise ValueError(
                "Don't know which state key '%s' requires." % key
            )

    def _switch_to_state(self, state, pointer):
        """Move from one state to the other.

        i.e. move from NORMAL to SHIFTED

        """
        if state == self.state:
            return

        # Clean this up, what about auto caps too?
        if self.state == KeyPad.State.NORMAL:
            expected_state = KeyPad.State.SHIFTED
        else:
            expected_state = KeyPad.State.NORMAL

        key_rect = self.get_key_position("shift")

        self._tap_key(key_rect, pointer)
        self.state.wait_for(expected_state)

    def _tap_key(self, key_rect, pointer):
        if pointer is not None:
            pointer.click_object(key_rect)
        else:
            self.pointer.click_object(key_rect)

    def press_key(self, key, pointer=None):
        """Taps key *key* with *pointer*

        If no pointer is passed in one is created for this purpose

        raises *ValueError* if *key* is not contained within this KeyPad.
        raises *RuntimeError* if this KeyPad is not visible.

        """
        if not self.contains_key(key):
            raise ValueError(
                "Key '%s' is not contained by this KeyPad (%s),"
                " cannot press it." % (key, self.objectName)
            )

        if pointer is None:
            pointer = pointer = Pointer(Touch.create())

        if not self.visible:
            raise RuntimeError(
                "This Keypad (%s) is not visible" % self.objectName
            )

        required_state = self._get_keys_required_keypad_state(key)

        self._switch_to_state(required_state, pointer)

        key_rect = self.get_key_position(key)
        self._tap_key(key_rect, pointer)

    def get_key_position(self, key):
        """Returns Key.Pos for the key with *key*.

        raises *ValueError* if unable to find the stored position for *key*
          (i.e. not contained within this KeyPad.)

        """
        key_rect = self._key_pos.get(key)

        if key_rect is None:
            raise ValueError("Unknown position for key '%s'" % key)

        return key_rect
