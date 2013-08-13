from time import sleep

from autopilot.input import Pointer, Touch
from autopilot.introspection import get_proxy_object_for_existing_process


# Definitions of enums used within the cpp source code.
KB_STATE_DEFAULT = 0
KB_STATE_SHIFTED = 1
KB_STATE_SYMBOL_1 = 2
KB_STATE_SYMBOL_2 = 3

ACTION_SHIFT = 1
ACTION_BACKSPACE = 2
ACTION_SPACE = 3
ACTION_SYM = 6
ACTION_RETURN = 7
ACTION_SWITCH = 11

# Moved '/' to primary symbol, default layout can have a .com instead.
default_keys = "qwertyuiopasdfghjklzxcvbnm."
shifted_keys = "QWERTYUIOPASDFGHJKLZXCVBNM."
primary_symbol = "1234567890*#+-=()!?@~/\\';:,."
# This is _not_ a complete list for this screen.
secondary_symbol = "$%<>[]`^|_{}\"&"

# Are we able to reset the keyboard some how? So that there is a addCleanup
# that resets the keyboard to a known state? This means that we won't have to
# ensure in setUp that we are in the correct state etc.

class OSKUnsupportedKey(RuntimeError):
    pass


class OSK(object):

    def __init__(self, pointer=None):
        try:
            maliit = get_proxy_object_for_existing_process(
                connection_name='org.maliit.server'
            )
        except RuntimeError:
            raise RuntimeError(
                "Unable to find maliit-server dbus object. Has it been started"
                "with introspection enabled?"
            )
        self.keyboard = maliit.select_single("Keyboard")
        self.keypad = maliit.select_single(
            "QQuickItem",
            objectName="keyboardKeypad"
        )

        # Contains instructions on how to move the keyboard into a specific
        # state/layout so that we can successfully press the required key.
        self._state_lookup_table = self._generate_state_lookup_table()

        if pointer is None:
            self.pointer = Pointer(Touch.create())
        else:
            self.pointer = pointer

    def dismiss(self):
        """Attempt to swipe the keyboard down so that it is hidden."""
        if self.is_available:
            x, y, h, w = self.keyboard.globalRect
            x_pos = int(w / 2)
            # start_y: just inside the keyboard, must be a better way than +1px
            start_y = y + 1
            end_y = y + int(h / 2)
            self.pointer.drag(x_pos, start_y, x_pos, end_y)

            self.keyboard.state.wait_for("HIDDEN")

    def is_available(self):
        """Returns wherever the keyboard is shown and ready to use."""
        return (
            self.keyboard.state == "SHOWN"
            and self.keyboard.hideAnimationFinished == False
        )


    def press_key(self, key):
        if not self.is_available():
            raise RuntimeError("Keyboard is not on screen")

        if self._is_special_key(key):
            self._press_special_key(key)
        else:
            required_state_for_key = self._get_keys_required_state(key)
            self._switch_keyboard_to_state(required_state_for_key)

            key = self.keypad.select_single('QQuickText', text=key)
            self.pointer.click_object(key)


    def type(self, string, delay=0.1):
        """Only 'normal' or single characters can be typed this way."""
        for char in string:
            self.press_key(char)
            sleep(delay)

    def _get_keys_required_state(self, char):
        """Given a character determine which state the keyboard needs to be in
        so that it is visible and can be clicked.

        """
        # '.' as it is available on all screens (currently)
        # '/' is available on more than 1 screen.
        if char in default_keys:
            return KB_STATE_DEFAULT
        elif char in shifted_keys:
            return KB_STATE_SHIFTED
        elif char in primary_symbol:
            return KB_STATE_SYMBOL_1
        elif char in secondary_symbol:
            return KB_STATE_SYMBOL_2
        else:
            raise OSKUnsupportedKey(
                "Don't know which state key '%s' requires" % char
            )

    def _switch_keyboard_to_state(self, target_state):
        """Given a target_state, presses the required keys to bring the
        keyboard into the correct state.

        :raises: *RuntimeError* if unable to change the keyboard into the
          expected state.

        """
        current_state = self.keyboard.layoutState

        if target_state == current_state:
            return

        instructions = self._state_lookup_table[target_state].get(
            current_state,
            None
        )
        if instructions is None:
            raise RuntimeError(
                "Don't know how to get to state %d from current state (%d)"
                % (target_state, current_state)
            )

        for step in instructions:
            key, expected_state = step
            # Veebers todo: just use press_key here, need to update the lookup
            # too.
            self._press_special_key(key)
            # self.press_key(key)
            self.keyboard.layoutState.wait_for(expected_state)

    def _press_special_key(self, key_label):
        """Press a named special key like Delete, Shift or ?123/ i.e. keys that
        don't necessarily have text names or are multi-character named.

        """

        key = None
        uppercase_key_label = key_label.upper()
        if uppercase_key_label == "\b":
            key = self.keypad.select_single(
                'QQuickText',
                action_type=ACTION_BACKSPACE
            )
        elif uppercase_key_label == "\n":
            key = self.keypad.select_single(
                'QQuickText',
                action_type=ACTION_RETURN
            )
        elif key_label == " ":
            key = self.keypad.select_single(
                'QQuickText',
                action_type=ACTION_SPACE
            )
        elif uppercase_key_label == "SHIFT":
            key = self.keypad.select_single(
                'QQuickText',
                action_type=ACTION_SHIFT
            )
        elif key_label in ["?123", "ABC", "1/2", "2/2"]:
            # These aren't really special keys, now the xpathselect bug has
            # been sorted.
            key = self.keypad.select_single('QQuickText', text=key_label)

        if key is None:
            raise OSKUnsupportedKey(
                "Attempting to push an unknown 'special' key: '%s'" % key_label
            )

        # Get required state for the key to be pushed (i.e not '\b', '\n', ' '
        # as they are always available.)

        self.pointer.click_object(key)

    def _is_special_key(self, key):
        return key in ["\n", "\b", " ", "SHIFT", "?123", "ABC", "1/2", "2/2"]

    # Give the state that you want and the current state, get instructions on how
    # to move to that state.
    # lookup_table[REQUESTED_STATE][CURRENT_STATE] -> Instructions(Key to press,
    #   Expected state after key press.)
    def _generate_state_lookup_table(self):
        state_lookup_table = [
            # KB_STATE_DEFAULT
            {
                KB_STATE_SHIFTED: [
                    ("SHIFT", KB_STATE_DEFAULT)
                ],
                KB_STATE_SYMBOL_1: [
                    ("ABC", KB_STATE_DEFAULT)
                ],
                KB_STATE_SYMBOL_2: [
                    ("ABC", KB_STATE_DEFAULT)
                ],
            },
            # KB_STATE_SHIFTED
            {
                KB_STATE_DEFAULT: [
                    ("SHIFT", KB_STATE_SHIFTED)
                ],
                KB_STATE_SYMBOL_1: [
                    ("ABC", KB_STATE_DEFAULT),
                    ("SHIFT", KB_STATE_SHIFTED)
                ],
                KB_STATE_SYMBOL_2: [
                    ("ABC", KB_STATE_DEFAULT),
                    ("SHIFT", KB_STATE_SHIFTED)
                ],
            },
            # KB_STATE_SYMBOL_1
            {
                KB_STATE_DEFAULT: [
                    ("?123", KB_STATE_SYMBOL_1)
                ],
                KB_STATE_SHIFTED: [
                    ("?123", KB_STATE_SYMBOL_1)
                ],
                KB_STATE_SYMBOL_2: [
                    ("2/2", KB_STATE_SYMBOL_1)
                ],
            },
            # KB_STATE_SYMBOL_2
            {
                KB_STATE_DEFAULT: [
                    ("?123", KB_STATE_SYMBOL_1),
                    ("1/2", KB_STATE_SYMBOL_2)
                ],
                KB_STATE_SHIFTED: [
                    ("?123", KB_STATE_SYMBOL_1),
                    ("1/2", KB_STATE_SYMBOL_2)
                ],
                KB_STATE_SYMBOL_1: [
                    ("1/2", KB_STATE_SYMBOL_2)
                ],
            },
        ]

        return state_lookup_table
