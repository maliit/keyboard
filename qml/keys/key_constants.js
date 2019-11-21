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

var tabletKeyMargins  =   0.5;    // gu
var phoneKeyMargins = 0.25 // gu
var fontSize    =   2.5;    // gu
var fontFamily  =   "Ubuntu";
var fontColor   =   "#333333";
var fontBold    =   false;
var selectionColor = "#19B6EE";

var backgroundColor = "#f7f7f7";
var dividerColor = "#cdcdcd";

var tabletAnnotationFontSize = 10; // dp
var phoneAnnotationFontSize = 10; // dp
var annotationTopMargin = 0.35; // gu
var annotationRightMargin = 0.65; // gu
var annotationFontColor = "#333333";
var annotationFont = "Ubuntu";

var spaceOpacity = 0.33;

/* magnifier */
var magnifierHorizontalPadding = 1.8; // gu, stretch the magnifier a little bit in x-axis
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
var popupBorderColor = "#888888"

var keyBorderEnabled = false

var charKeyColor          = "white"
var charKeyPressedColor   = "#d9d9d9"
var charKeyBorderColor = "#888888"

var actionKeyColor        = "#cdcdcd"
var actionKeyPressedColor = "#aeaeae"
var actionKeyBorderColor = "#888888"

var top_margin = 1;  // gu
var bottom_margin = 1; // gu
var tabletRowMargin = 1; // gu
var phoneRowMarginLandscape = 4; // dp
var phoneRowMarginPortrait = 7; // dp

var urlLayoutLeftSpacerSize = 5; // gu
var emailLayoutUrlKeyPadding = 1.5; // gu

/* language menu */
var languageMenuListViewPadding = 2.22; // gu

var tabletWordribbonHeight = 6; // gu
var phoneWordribbonHeight = 4; // gu
var tabletWordRibbonFontSize = 17 // dp
var phoneWordRibbonFontSize = 14 // dp

var phoneKeyboardHeightPortrait = 0.40; // percent of screen
var phoneKeyboardHeightLandscape = 0.49; // percent of screen

var tabletKeyboardHeightPortrait = 0.31; // percent of screen
var tabletKeyboardHeightLandscape = 0.34; // percent of screen

/* for UITK Components */
var toolkitTheme = "Ubuntu.Components.Themes.Ambiance" 
