/*
 * Copyright 2016 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import keys 1.0

import "parser.js" as Parser

ActionKey {
    iconNormal: "erase";
    iconShifted: "erase";
    iconCapsLock: "erase";
    action: "backspace";

    property string preedit: maliit_input_method.preedit
    property string m_preedit: ""
    property string syllable_preedit: ""
    property string last_preedit: ""
    property bool isPreedit: preedit != ""

    overridePressArea: true;

    onReleased: {
        if (isPreedit) {
            if (preedit.length > 1){ /* at least 2 length */
                syllable_preedit = preedit.substring(0,preedit.length - 1);
                last_preedit = preedit[preedit.length - 1]; /* last jamo or syllable */

                m_preedit = Parser.erase_jamo(last_preedit);
                if (m_preedit != ""){ /* exsit jamo */
                    maliit_input_method.preedit = syllable_preedit + m_preedit;
                } else {
                    maliit_input_method.preedit = syllable_preedit;
                }
            } else {
                  if (Parser.is_syllable(preedit)){ /* preedit is one syllable */
                    m_preedit = Parser.erase_jamo(preedit);
                    maliit_input_method.preedit = m_preedit;
                  } else { /* it is only jamo like "ㄱ" or "ㅏ" */
                     event_handler.onKeyReleased("", action);
                  }
            }
        } else {
             event_handler.onKeyReleased("", action);
        }
    }

    onPressed: {
        if (maliit_input_method.useAudioFeedback)
            audioFeedback.play();

        if (maliit_input_method.useHapticFeedback)
            pressEffect.start();

        if (!isPreedit) {
            event_handler.onKeyPressed("", action);
        } else {
            if (preedit.length == 1 && !Parser.is_syllable(preedit)) /* fixed erase action repeat */
                event_handler.onKeyPressed("", action);
        }
    }

    onPressAndHold: {
        return;
    }


}
