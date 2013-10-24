/*
 * Copyright 2013 Canonical Ltd.
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

import QtQuick 2.0
import Ubuntu.Components 0.1


/**
 * these items only hold variables
 */

Item {
     id: keyboard_constants

     property real key_area_borders:                        units.gu(  1.222    );
     property string font_color:                            "#666666"
     property string font_family:                           "Ubuntu Medium"

     property int  reference_width:                         720
     property int  reference_height:                        468

     property int  portrait_invisible_touch_area:           units.gu(  6.777    );
     property int  portrait_keyboard_visible_height:        units.gu( 26.0      );
     property real portrait_keyboard_screen_width_ratio:               1.0

     property real portrait_top_margin:                     units.gu(  1.35     );
     property real landscape_top_margin:                    units.gu(  4.35     );
     property real portrait_bottom_margin:                  units.gu(  2.00     );
     property real landscape_bottom_margin:                 units.gu(  2.00     );

     property real key_height:                              units.gu(  4.888     ); // not used

     // make sure when you change these, also change the unittest
     property real key_width_small:                         10
     property real key_width_medium:                        12
     property real key_width_large:                         15                   // shift
     property real key_width_xlarge:                        18                   // 123 key
     property real key_width_xxlarge:                       24                   // space
     property real key_width_stretched:                     30                   // ?

     property real font_size:                               12;
     property real font_size_small:                         10;

     property int  landscape_invisible_touch_area:          units.gu(  6.777    );
     property int  landscape_keyboard_visible_height:       units.gu( 33.4      );
     property real landscape_keyboard_screen_width_ratio:              1.0

     property real landscape_space_between_rows:            units.dp(  0.00     );
     property real landscape_space_between_keys:            units.dp(  0.00     );
     property real portrait_space_between_rows:             units.dp(  2.00     );
     property real portrait_space_between_keys:             units.dp( 10.00     );

     property string key_background_normal:                 "keybg@18.png"
     property string key_background_special:                "keybg_action@18.png"
     property string key_background_deadkey:                ""

     property real phone_keyboard_height_portrait:  0.365;
     property real phone_keyboard_height_landscape: 0.50;

     property real tablet_keyboard_height_portrait:  0.28;
     property real tablet_keyboard_height_landscape: 0.38;

     property int landscape_wordribbon_height:              units.gu( 4.0 );
     property int portrait_wordribbon_height:               units.gu( 4.0 );
}

