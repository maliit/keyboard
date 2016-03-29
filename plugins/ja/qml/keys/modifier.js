/*
 * Copyright 2015 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

.pragma library

/*
 * Modified character map for ModifierKey
 *
 * ModifierKey modify a charcter by left/right/up swipe. This is character map
 * and swipe direction.
 *   map[ORIGIN KEY] = [NO MODIFY, LEFT, UP, RIGHT]
 */
var map = {}
map["あ"] = ["あ", "", "ぁ", ""]
map["い"] = ["い", "", "ぃ", ""]
map["う"] = ["う", "ヴ", "ぅ", ""]
map["え"] = ["え", "", "ぇ", ""]
map["お"] = ["お", "", "ぉ", ""]
map["か"] = ["か", "が", "", ""]
map["き"] = ["き", "ぎ", "", ""]
map["く"] = ["く", "ぐ", "", ""]
map["け"] = ["け", "げ", "", ""]
map["こ"] = ["こ", "ご", "", ""]
map["さ"] = ["さ", "ざ", "", ""]
map["し"] = ["し", "じ", "", ""]
map["す"] = ["す", "ず", "", ""]
map["せ"] = ["せ", "ぜ", "", ""]
map["そ"] = ["そ", "ぞ", "", ""]
map["た"] = ["た", "だ", "", ""]
map["ち"] = ["ち", "ぢ", "", ""]
map["つ"] = ["つ", "づ", "っ", ""]
map["て"] = ["て", "で", "", ""]
map["と"] = ["と", "ど", "", ""]
map["は"] = ["は", "ば", "", "ぱ"]
map["ひ"] = ["ひ", "び", "", "ぴ"]
map["ふ"] = ["ふ", "ぶ", "", "ぷ"]
map["へ"] = ["へ", "べ", "", "ぺ"]
map["ほ"] = ["ほ", "ぼ", "", "ぽ"]
map["や"] = ["や", "", "ゃ", ""]
map["（"] = ["（", "「", "『", "【"]
map["ゆ"] = ["ゆ", "", "ゅ", ""]
map["）"] = ["）", "」", "』", "】"]
map["よ"] = ["よ", "", "ょ", ""]
map["わ"] = ["わ", "ゐ", "ゎ", "ゑ"]
map["ー"] = ["ー", "＝", "～", "＋"]
map["、"] = ["、", "・", "…", "＠"]
map["。"] = ["。", "：", "；", "♪"]

/*
 * Reverse map for ModifierKey map
 *
 * This map is reverse map for modified characters.
 */
var normalize = {}
normalize["ぁ"] = "あ"
normalize["ぃ"] = "い"
normalize["ヴ"] = "う"; normalize["ぅ"] = "う"
normalize["ぇ"] = "え"
normalize["ぉ"] = "お"
normalize["が"] = "か"
normalize["ぎ"] = "き"
normalize["ぐ"] = "く"
normalize["げ"] = "け"
normalize["ご"] = "こ"
normalize["ざ"] = "さ"
normalize["じ"] = "し"
normalize["ず"] = "す"
normalize["ぜ"] = "せ"
normalize["ぞ"] = "そ"
normalize["だ"] = "た"
normalize["ぢ"] = "ち"
normalize["づ"] = "つ"; normalize["っ"] = "つ"
normalize["で"] = "て"
normalize["ど"] = "と"
normalize["ば"] = "は"; normalize["ぱ"] = "は"
normalize["び"] = "ひ"; normalize["ぴ"] = "ひ"
normalize["ぶ"] = "ふ"; normalize["ぷ"] = "ふ"
normalize["べ"] = "へ"; normalize["ぺ"] = "へ"
normalize["ぼ"] = "ほ"; normalize["ぽ"] = "ほ"
normalize["ゃ"] = "や"
normalize["「"] = "（"; normalize["『"] = "（"; normalize["【"] = "（"
normalize["ゅ"] = "ゆ"
normalize["」"] = "）"; normalize["』"] = "）"; normalize["】"] = "）"
normalize["ょ"] = "よ"
normalize["ゐ"] = "わ"; normalize["ゎ"] = "わ"; normalize["ゑ"] = "わ"
normalize["＝"] = "ー"; normalize["～"] = "ー"; normalize["＋"] = "ー"
normalize["・"] = "、"; normalize["…"] = "、"; normalize["＠"] = "、"
normalize["："] = "。"; normalize["；"] = "。"; normalize["♪"] = "。"

