# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
#
# Ubuntu Keyboard Test Suite
# Copyright (C) 2012-2013 Canonical
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

from autopilot.input import Pointer, Touch


class WordRibbon(object):
    """Emulator to wrap some of the complexities of the word ribbon."""

    def __init__(self, word_ribbon):
        self._word_ribbon = word_ribbon

    def is_enabled(self):
        return self._word_ribbon.visible

    def select_word_item(self, word_item=None, text=None, pointer=None):
        if word_item is None and text is None:
            raise RuntimeError("Need something here buddy.")

        if word_item is None:
            try:
                word_item = self.get_suggestions(text=text)[0]
            except IndexError:
                raise RuntimeError(
                    "Unable to find the requested word item with text '%s'"
                    % text
                )

        if pointer is None:
            pointer = Pointer(Touch.create())

        if not word_item.is_visible():
            self.drag_word_item_visible(word_item, pointer)
        pointer.click_object(word_item)

    def drag_word_item_visible(self, word_item, pointer=None):
        # What about just passing in the word_item text?

        #ribbon width is the same as screen width (for now).
        rx, ry, rw, rh = self._word_ribbon.globalRect
        ix, ir, iw, ih = word_item.globalRect

        item_end = ix + iw
        if item_end <= rw:
            # actually a simply check would be it's focus.
            # No need to swipe it into the clear, should already be there.
            return False
        item_difference = item_end - rw

        if item_difference > rw:
            # Split it up into smaller chunks.
            pass

        y_coord = ry + int(rh/2)  # HoriCenter of the ribbonbar
        x_start = rw
        x_end = rw - item_difference

        if pointer is None:
            pointer = Pointer(Touch.create())
        print "Dragging: %d, %d, %d, %d" % (x_start, y_coord, x_end, y_coord)
        pointer.drag(x_start, y_coord, x_end, y_coord)

    def get_suggestions(self, text=None):
        word_list = self._word_ribbon.select_single(
            'QQuickListView',
            objectName='wordListView'
        )
        # The z attrib is there so we only get suitable candidates. Perhaps
        # there is a better way in which to do this?
        if text is None:
            word_items = word_list.select_many('QQuickItem', z=1.0)
        else:
            word_items = word_list.select_many(
                'QQuickItem',
                z=1.0,
                word_text=text
            )
        return [WordItem(i) for i in word_items]


# Store the actual item but wrap a couple so that it's easier to query.
# For instance have a property globalRect that just returns the correct items
# globalrect. Property for text etc.
class WordItem(object):
    """This class wraps some of the complexities of the word items UI tree.

    It allows a user an easy way to check the text of the (nestled) word item
    as well as click on the suggestion itself.

    """

    def __init__(self, word_item):
        # The word item will contain 2 children
        # 0: QuickItem (and then some)
        # 1: MouseArea
        self._word_item = word_item
        self.text = word_item.word_text

    @property
    def globalRect(self):
        return self._word_item.select_single("QQuickMouseArea").globalRect

    def is_visible(self):
        return self._word_item.focus
