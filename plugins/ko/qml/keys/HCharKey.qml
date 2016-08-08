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
import QtMultimedia 5.0
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3

import keys 1.0
import "key_constants.js" as UI
import "parser.js" as Parser

CharKey {

    allowPreeditHandler: true
    preeditHandler: handler

    Item {
        id: handler

        //Each Hangul represents one syllable. The Hangul are composed of jamo. 
        //It need to manipulate preedit until compose one syllable. 

        function onKeyReleased(keyString, action) {
            // get previous preedit string
            var preedit = maliit_input_method.preedit;

            if (Parser.is_hangul(keyString)) {
                // parsing preedit until compose one syllable. 
                if (preedit.length > 1) { 
                    var syllableString = preedit.substring(0,preedit.length - 1);
                    var preeditString = preedit[preedit.length - 1];
                    maliit_input_method.preedit = syllableString + Parser.add_jamo(preeditString, keyString);
                } else {
                    maliit_input_method.preedit = Parser.add_jamo(preedit, keyString);
                }

                return;
            }

            maliit_input_method.preedit = preedit + keyString;
            event_handler.onKeyReleased("", "commit");
        }
    }
}
