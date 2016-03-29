/*
 * Copyright 2015 Canonical Ltd.
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
import "keys/"
import keys 1.0

KeyPad {
    anchors.fill: parent

    content: c1

    Column {
        id: c1
        anchors.fill: parent
        spacing: 0

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            UndoKey { leftSide: true; width: panel.keyWidth; }
            FlickCharKey {
                label: layout.state == "kana" ? "あ" : "@ / : _";
                leaves: layout.state == "kana" ? ["あ", "い", "う", "え", "お"] : ["@", "/", ":", "_", "1"];
                annotation: layout.state == "kana" ? "" : "1";
            }
            FlickCharKey {
                label: layout.state == "kana" ? "か" : "ABC";
                leaves: layout.state == "kana" ? ["か", "き", "く", "け", "こ"] : ["a", "b", "c", "#", "2"];
                annotation: layout.state == "kana" ? "" : "2#";
            }
            FlickCharKey {
                label: layout.state == "kana" ? "さ" : "DEF";
                leaves: layout.state == "kana" ? ["さ", "し", "す", "せ", "そ"] : ["d", "e", "f", "$", "3"];
                annotation: layout.state == "kana" ? "" : "3$";
            }
            BackspaceKey { rightSide: true; width: panel.keyWidth; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            CursorKey { action: "left"; }
            FlickCharKey {
                label: layout.state == "kana" ? "た" : "GHI";
                leaves: layout.state == "kana" ? ["た", "ち", "つ", "て", "と"] : ["g", "h", "i", "(", "4"];
                annotation: layout.state == "kana" ? "" : "4(";
            }
            FlickCharKey {
                label: layout.state == "kana" ? "な" : "JKL";
                leaves: layout.state == "kana" ? ["な", "に", "ぬ", "ね", "の"] : ["j", "k", "l", "\"", "5"];
                annotation: layout.state == "kana" ? "" : "5\"";
            }
            FlickCharKey {
                label: layout.state == "kana" ? "は" : "MNO";
                leaves: layout.state == "kana" ? ["は", "ひ", "ふ", "へ", "ほ"] : ["m", "n", "o", ")", "6"];
                annotation: layout.state == "kana" ? "" : "6)";
            }
            CursorKey { action: "right"; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            LanguageKey    { id: languageMenuButton; height: panel.keyHeight; }
            FlickCharKey {
                label: layout.state == "kana" ? "ま" : "PQRS"
                leaves: layout.state == "kana" ? ["ま", "み", "む", "め", "も"] : ["p", "q", "r", "s", "7"]
                annotation: layout.state == "kana" ? "" : "7"
            }
            FlickCharKey {
                label: layout.state == "kana" ? "や" : "TUV"
                leaves: layout.state == "kana" ? ["や", "（", "ゆ", "）", "よ"] : ["t", "u", "v", "'", "8"]
                annotation: layout.state == "kana" ? "" : "8'"
            }
            FlickCharKey {
                label: layout.state == "kana" ? "ら" : "WXYZ"
                leaves: layout.state == "kana" ? ["ら", "り", "る", "れ", "ろ"] : ["w", "x", "y", "z", "9"]
                annotation: layout.state == "kana" ? "" : "9"
            }
            ActionKey { label: "記号"; action: "symbol"; width: panel.keyWidth; }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 0

            KanaSwitchKey { id: layout; }
            ModifierKey { layoutState: layout.state; }
            FlickCharKey {
                label: layout.state == "kana" ? "わ" : "- + *"
                leaves: layout.state == "kana" ? ["わ", "を", "ん", "ー"] : ["-", "+", "0", "*"];
                annotation: layout.state == "kana" ? "" : "0";
            }
            FlickCharKey {
                label: layout.state == "kana" ? "、。" : ". , ! ?";
                leaves: layout.state == "kana" ? ["、", "。", "！", "？"] : [".", ",", "!", "?"];
            }
            CommitKey    { id: enterKey; width: panel.keyWidth; }
        }
    } // column
}
