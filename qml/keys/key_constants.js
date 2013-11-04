/*
 * Copyright 2012 Canonical Ltd.
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

var keyMargins  =   1.8;    // dp
var fontSize    =   2;    // gu
var fontFamily  =   "Ubuntu Medium";
var fontColor   =   "#666666"
var fontBold    =   true

var annotationFontSize = 0.85; // gu
var annotationMargins = 0.7; // gu
var annotationFontColor = "#88666666"

/* magnifier */
var magnifierHorizontalPadding = 1; // gu, stretch the magnifier a little bit in x-axis
var magnifierVerticalPadding = 1; // gu, stretch the magnifier a little bit in y-axis

/* special keys */
var actionKeyPadding = 2;  // gu, action keys are a bit bigger
var symbolShiftKeyFontSize = 2; // gu
var smallFontSize = 1.5; // gu, for keys that show more than one char, e.g. '.com'

/* extended keys */
var popoverCellPadding = 2.2; // gu
var popoverTopMargin   = 10; // dp
var popoverEdgeMargin = 2.2; // gu
var popoverSquat      = 3; // gu, when no wordribbon, avoid click-through above input trap

var imageCharKey        = "../images/key_bg@27.png"
var imageCharKeyPressed = "../images/keybg_action_active@18.png"

var imageActionKey        = "../images/action_key_bg@27.png"
var imageActionKeyPressed = "../images/keybg_action_active@18.png"

var top_margin = 1.35;  // gu
var bottom_margin = 1.00; // gu


var urlLayoutLeftSpacerSize = 5; // gu
var emailLayoutUrlKeyPadding = 4; // gu

/* language menu */
var languageMenuListViewPadding = 2.22; // gu
var languageMenuCorner = 2.5; // gu
