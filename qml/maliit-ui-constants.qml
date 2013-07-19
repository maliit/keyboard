

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

     property int  reference_width:                         units.gu( 40.0      );
     property int  reference_height:                        units.gu( 26.0      );

     property int  portrait_invisible_touch_area:           units.gu(  6.777    );
     property int  portrait_keyboard_visible_height:        units.gu( 26.0      );
     property real portrait_keyboard_screen_width_ratio:               1.0

     property real portrait_top_margin:                     units.gu(  1.35     );
     property real landscape_top_margin:                    units.gu(  4.35     );
     property real portrait_bottom_margin:                  units.gu(  2.00     );
     property real landscape_bottom_margin:                 units.gu(  2.00     );

     property real key_height:                              units.gu(  4.888     );

     property real key_width_small:                         units.gu(  3.166     );
     property real key_width_medium:                        units.gu(  3.666     );
     property real key_width_large:                         units.gu(  4.222     ); // shift
     property real key_width_xlarge:                        units.gu(  6.0       ); // 123 key
     property real key_width_xxlarge:                       units.gu(  17.0      ); // space
     property real key_width_stretched:                     units.gu(  3.0       ); // ?

     property real font_size:                               units.gu(  2.111     );
     property real font_size_small:                         units.gu(  1.8       );

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

