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

logger = logging.getLogger(__name__)


class KeyPadState:
    NORMAL = "NORMAL"
    SHIFTED = "SHIFTED"
    CAPSLOCK = "CAPSLOCK"


class KeyPad(UbuntuKeyboardEmulatorBase):
    """A basic emulator that provides the details of the keys contained within.

    """

    def get_key_details(self):
        contained_keys = {}
        key_positions = {}

        def _iter_keys(key_type, label_fn):
            for key in self.select_many(key_type):
                with key.no_automatic_refreshing():
                    key_pos = Key.Pos(*key.globalRect)
                    label = label_fn(key)
                    if label != '':
                        contained_keys[label] = KeyPadState.NORMAL
                        key_positions[label] = key_pos
                    if key.shifted != '':
                        contained_keys[key.shifted] = KeyPadState.SHIFTED
                        key_positions[key.shifted] = key_pos

        _iter_keys("CharKey", lambda x: x.label)
        _iter_keys("ActionKey", lambda x: x.action)

        return (contained_keys, key_positions)
