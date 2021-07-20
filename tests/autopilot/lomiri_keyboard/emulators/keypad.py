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

import logging

import lomiriuitoolkit

from lomiri_keyboard.emulators.key import Key


logger = logging.getLogger(__name__)


class KeyPadState:
    NORMAL = "NORMAL"
    SHIFTED = "SHIFTED"
    CAPSLOCK = "CAPSLOCK"


class KeyPad(lomiriuitoolkit.LomiriUIToolkitCustomProxyObjectBase):
    """A basic emulator that provides the details of the keys contained within.

    """

    def get_key_details(self):
        contained_keys = {}
        key_positions = {}

        def _iter_keys(key_type, label_fn):
            for key in self.select_many(key_type):
                with key.no_automatic_refreshing():
                    rect = key.globalRect
                    if key.leftSide:
                        rect = (
                            key.globalRect[0] + key.leftOffset,
                            key.globalRect[1],
                            key.globalRect[2] - key.leftOffset,
                            key.globalRect[3])
                    elif key.rightSide:
                        rect = (
                            key.globalRect[0],
                            key.globalRect[1],
                            key.globalRect[2] - key.rightOffset,
                            key.globalRect[3])
                    key_pos = Key.Pos(*rect)
                    label = label_fn(key)
                    if label != '':
                        contained_keys[label] = KeyPadState.NORMAL
                        key_positions[label] = key_pos
                    if hasattr(key, "shifted") and key.shifted != '':
                        contained_keys[key.shifted] = KeyPadState.SHIFTED
                        key_positions[key.shifted] = key_pos

        _iter_keys("CharKey", lambda x: x.label)
        _iter_keys("FlickCharKey", lambda x: x.label)
        _iter_keys("ActionKey", lambda x: x.action)
        _iter_keys("ShiftKey", lambda x: x.action)
        _iter_keys("LanguageKey", lambda x: x.action)
        _iter_keys("ReturnKey", lambda x: x.action)
        _iter_keys("CommitKey", lambda x: x.action)
        _iter_keys("HCharKey", lambda x: x.label)

        return (contained_keys, key_positions)
